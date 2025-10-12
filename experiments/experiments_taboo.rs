use std::{fs, process::Command, thread, sync::{Arc, Mutex}, io::Write};

const GRAPH_DIR: &str = "../grafos/";
const EXECUTABLE: &str = "../Taboo.out";
const OUTPUT_CSV: &str = "../experiments/results/experiments_taboo.csv";
const N_REPS: usize = 50;
const MAX_THREADS: usize = 5; // Usa todos los núcleos menos uno
const TABU_LENS: [usize; 4] = [0, 5, 20, 100];

fn main() {
    //  Semaforo que controla cuántos hilos pueden estar activos al mismo tiempo
    let semaphore = Arc::new(Mutex::new(0usize));

    let entries = fs::read_dir(GRAPH_DIR).expect("No se pudo leer el directorio de grafos");

    let mut file_csv_raw = std::fs::OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(OUTPUT_CSV)
        .expect("No se pudo crear/abrir el archivo CSV");
    writeln!(file_csv_raw, "respuesta;tiempo;densidad;vertices").expect("No se pudo escribir el encabezado");
    let file_csv = Arc::new(Mutex::new(file_csv_raw));

    let mut handles = vec![];

    for entry in entries {
        let path = entry.expect("Error leyendo archivo").path();

        if !path.is_file() {continue;}

        // Solo procesa archivos con extensión .graph
        match path.extension() {
            Some(ext) if ext == "graph" => {},
            _ => continue,
        }

        let graph_path = path.to_str().unwrap().to_string();
        for &tabu_len in &TABU_LENS {
            println!("Probando: archivo={}, tabu_len={}", graph_path, tabu_len);
            for _ in 0..N_REPS {
                let semaphore = Arc::clone(&semaphore);
                let file_csv = Arc::clone(&file_csv);
                let graph_path = graph_path.clone();

                // Controla el número de hilos activos
                loop {
                    let mut active = semaphore.lock().unwrap();
                    if *active < MAX_THREADS {
                        *active += 1;
                        break;
                    }
                    drop(active);
                    thread::sleep(std::time::Duration::from_millis(50));
                }

                handles.push(thread::spawn(move || {
                    let output = Command::new(EXECUTABLE)
                        .arg("--input").arg(&graph_path)
                        .arg("--time_limit").arg("3")
                        .arg("--tabu_len").arg(tabu_len.to_string())
                        .output()
                        .expect("Fallo al ejecutar el experimento");
                    let result = String::from_utf8_lossy(&output.stdout);

                    // Extrae n* y c* del nombre de archivo
                    let file_name = std::path::Path::new(&graph_path)
                        .file_name()
                        .and_then(|f| f.to_str())
                        .unwrap_or("");
                    let n = file_name.split('_')
                        .find(|s| s.starts_with("n"))
                        .and_then(|s| s[1..].parse::<usize>().ok())
                        .unwrap_or(0);
                    let c = file_name.split('_')
                        .find_map(|s| {
                            if let Some(idx) = s.find('c') {
                                let dens = &s[idx+1..];
                                dens.parse::<f64>().ok()
                            } else {
                                None
                            }
                        })
                        .unwrap_or(0.0);

                    // Escribe la salida en el archivo CSV, agregando n y c
                    let mut file_csv = file_csv.lock().unwrap();
                    writeln!(file_csv, "{};{};{}", result.trim(), c, n).expect("No se pudo escribir en el CSV");

                    // Libera el semáforo al terminar
                    let mut active = semaphore.lock().unwrap();
                    *active -= 1;
                }));
            }
        }
    }

    // Espera a que todos los threads terminen
    for handle in handles {
        handle.join().unwrap();
    }
}