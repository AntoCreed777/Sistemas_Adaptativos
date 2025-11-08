#!/usr/bin/env bash
# Ejecuta BRKGA sobre todas las instancias Erdos usando GNU parallel

set -Eeuo pipefail

# --- Parámetros personalizables ---------------------------------------------
WORKDIR=${1:-$(pwd)}      # 1er argumento: ruta base del experimento
DATA_DIR="${WORKDIR}/grafos"
OUT_DIR="${WORKDIR}/experiments/results"
OUT_CSV="${OUT_DIR}/experiments_brkga_60s.csv"
EXE_POP="${WORKDIR}/BRKGA.out"
CONF_FILE="${WORKDIR}/brkga.conf"
TIME_LIMIT=60

# Número máximo de ejecuciones en paralelo
MAX_JOBS=${MAX_JOBS:-12}

# --- Preparación -------------------------------------------------------------
mkdir -p "${OUT_DIR}"
# Encabezado con el mismo formato que antes
printf "respuesta;tiempo;estado_solucion;cantidad_vertices;densidad\n" > "${OUT_CSV}"

echo "[INFO] Directorio de trabajo: ${WORKDIR}"
echo "[INFO] Archivo de configuración: ${CONF_FILE}"
echo "[INFO] Ejecutable: ${EXE_POP}"
echo "[INFO] Tiempo límite: ${TIME_LIMIT}s"
echo "[INFO] Máx. trabajos en paralelo: ${MAX_JOBS}"

# --- Función para procesar UN solo .graph -----------------------------------
run_one() {
    local graph_file="$1"

    echo "[RUN] ${graph_file}"

    # Ejecutar BRKGA y capturar toda la salida
    local out last_line
    out=$("${EXE_POP}" -c "${CONF_FILE}" -t "${TIME_LIMIT}" -i "${graph_file}" 2>&1 || true)
    last_line=$(echo "${out}" | tail -n 1 | tr -d '\r')

    if [[ -z "${last_line}" ]]; then
        echo "[WARN] Salida vacía para ${graph_file}: ${out//$'\n'/ | }" >&2
        return 0
    fi

    # Extraer n y densidad d a partir del nombre:
    # Formato esperado: erdos_n<N>_p0c<D>_<id>.graph
    local fname rest fname_no_ext n d
    fname=$(basename "${graph_file}")
    fname_no_ext="${fname%.graph}"          # erdos_n1000_p0c0.05_1
    rest="${fname_no_ext#erdos_n}"          # 1000_p0c0.05_1
    rest="${rest#n}"                        # 1000_p0c0.05_1 (por si acaso)
    n="${rest%%_*}"                         # 1000
    rest="${rest#${n}_p0c}"                 # 0.05_1
    d="${rest%_*}"                          # 0.05

    # Guardar en el CSV con columnas extra
    printf '%s;%s;%s\n' "${last_line}" "${n}" "${d}" >> "${OUT_CSV}"
    echo "  [OK] línea guardada con n=${n}, densidad=${d}"
}

export EXE_POP CONF_FILE TIME_LIMIT OUT_CSV
export -f run_one

# --- Llamada paralela --------------------------------------------------------
# Busca archivos con patrón erdos_n*_p0c*_[id].graph en DATA_DIR
find "${DATA_DIR}" -maxdepth 1 -type f -name 'erdos_n*_p0c*_[0-9]*.graph' -print \
  | sort \
  | parallel -j "${MAX_JOBS}" run_one {}

echo "[DONE] Experimentos BRKGA ${TIME_LIMIT}s completados. Resultados en ${OUT_CSV}"
