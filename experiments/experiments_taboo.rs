use std::fs;
use std::process::Command;
use std::thread;
use std::sync::{Arc, Mutex};
use std::io::Write;

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
    writeln!(file_csv_raw, "respuesta,tiempo,densidad,vertices").expect("No se pudo escribir el encabezado");
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
        let file_csv = Arc::clone(&file_csv);
        let tabu_lens = TABU_LENS.to_vec();
        let semaphore = Arc::clone(&semaphore);

        for &tabu_len in &tabu_lens {
            for _ in 0..N_REPS {
                // Espera si hay demasiados hilos activos
                loop {
                    let mut active = semaphore.lock().unwrap();
                    if *active < MAX_THREADS {
                        *active += 1;
                        break;
                    }
                    drop(active);
                    std::thread::sleep(std::time::Duration::from_millis(50));
                }

                let graph_path = graph_path.clone();
                let file_csv = Arc::clone(&file_csv);
                let semaphore = Arc::clone(&semaphore);

                let handle = thread::spawn(move || {
                    let output = Command::new(EXECUTABLE)
                        .arg("--input").arg(&graph_path)
                        .arg("--time_limit").arg("3")
                        .arg("--tabu_len").arg(tabu_len.to_string())
                        .output()
                        .expect("Fallo al ejecutar el experimento");

                    let result = String::from_utf8_lossy(&output.stdout);

                    // Escribe la salida en el archivo CSV
                    let mut file_csv = file_csv.lock().unwrap();
                    writeln!(file_csv, "{}", result.trim()).expect("No se pudo escribir en el CSV");

                    // Libera el semáforo al terminar
                    let mut active = semaphore.lock().unwrap();
                    *active -= 1;
                });
                handles.push(handle);
            }
        }
    }

    // Espera a que todos los threads terminen
    for handle in handles {
        handle.join().unwrap();
    }
}