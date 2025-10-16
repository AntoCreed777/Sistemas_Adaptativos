use std::{
    fs,
    process::Command,
    sync::{Arc, Mutex},
    io::Write,
};
use threadpool::ThreadPool;

const GRAPH_DIR: &str = "../grafos/";
const EXECUTABLE: &str = "../Taboo.out";
const OUTPUT_CSV: &str = "../experiments/results/experiments_taboo.csv";
const TIME_LIMIT: &str = "10";
const N_REPS: usize = 30;
const MAX_THREADS: usize = 4; // Número de threads simultáneos
const TABU_LENS: [usize; 4] = [5, 20, 100, 500];

fn main() {
    // Crea el ThreadPool
    let pool = ThreadPool::new(MAX_THREADS);

    // Abre el CSV
    let mut file_csv_raw = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(OUTPUT_CSV)
        .expect("No se pudo crear/abrir el archivo CSV");
    writeln!(file_csv_raw, "respuesta;tiempo;tipo_resultado;taboo_len;densidad;vertices").expect("No se pudo escribir el encabezado");
    let file_csv = Arc::new(Mutex::new(file_csv_raw));

    // Itera sobre los archivos
    for entry in fs::read_dir(GRAPH_DIR).expect("No se pudo leer el directorio de grafos") {
        let path = entry.expect("Error leyendo archivo").path();

        if !path.is_file() {continue;}

        // Solo procesa archivos con extensión .graph
        match path.extension() {
            Some(ext) if ext == "graph" => {},
            _ => continue,
        }

        let graph_path = path.to_str().unwrap().to_string();

        for &tabu_len in &TABU_LENS {
            for _ in 0..N_REPS {
                let graph_path = graph_path.clone();
                let file_csv = Arc::clone(&file_csv);

                // Envía la tarea al ThreadPool
                pool.execute(move || {
                    let output = Command::new(EXECUTABLE)
                        .arg("--input").arg(&graph_path)
                        .arg("--time_limit").arg(TIME_LIMIT)
                        .arg("--tabu_len").arg(tabu_len.to_string())
                        .output()
                        .expect("Fallo al ejecutar el experimento");

                    let result = String::from_utf8_lossy(&output.stdout);

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
                            } else { None }
                        })
                        .unwrap_or(0.0);

                    // Escribe la salida en el CSV
                    let mut file_csv = file_csv.lock().unwrap();
                    writeln!(file_csv, "{};{};{};{}", result.trim(), tabu_len, c, n)
                        .expect("No se pudo escribir en el CSV");
                });
            }
        }
    }

    // Espera a que todas las tareas terminen
    pool.join();
    println!("Todos los experimentos han terminado, Kasuma. Tu CPU puede descansar ahora. 💥");
}
