#!/usr/bin/env bash
# Ejecuta BRKGA (MH poblacional) sobre instancias Erdos con límite TIME_LIMIT
# Permite establecer un directorio de trabajo personalizado.

set -Eeuo pipefail

# --- Parámetros personalizables ---------------------------------------------
WORKDIR=${1:-$(pwd)}      # 1er argumento: ruta base del experimento
DATA_DIR="${WORKDIR}/grafos"
OUT_DIR="${WORKDIR}/experiments/results"
OUT_CSV="${OUT_DIR}/experiments_brkga_10s.csv"
EXE_POP="${WORKDIR}/BRKGA.out"
CONF_FILE="${WORKDIR}/brkga.conf"
TIME_LIMIT=1

# --- Configuración de dataset ------------------------------------------------
N_LIST=(1000 2000 3000)

# IMPORTANTE: estos strings DEBEN coincidir EXACTAMENTE con los nombres de tus archivos
# ej.: erdos_n1000_p0c0.1_1.graph  -> densidad="0.1"
#      erdos_n1000_p0c0.15_3.graph -> densidad="0.15"
D_LIST=("0.05" "0.1" "0.15" "0.2" "0.25" "0.3" "0.35" "0.4" "0.45" \
        "0.5" "0.55" "0.6" "0.65" "0.7" "0.75" "0.8" "0.85" "0.9" "0.95")

ID_LIST=$(seq 1 32)

# --- Preparación -------------------------------------------------------------
mkdir -p "${OUT_DIR}"
# Encabezado con el mismo formato que el output original + columnas extra
printf "respuesta;tiempo;estado_solucion;cantidad_vertices;densidad\n" > "${OUT_CSV}"

echo "[INFO] Directorio de trabajo: ${WORKDIR}"
echo "[INFO] Archivo de configuración: ${CONF_FILE}"
echo "[INFO] Ejecutable: ${EXE_POP}"
echo "[INFO] Tiempo límite: ${TIME_LIMIT}s"

# --- Bucle principal ---------------------------------------------------------
for n in "${N_LIST[@]}"; do
  for d in "${D_LIST[@]}"; do
    for id in ${ID_LIST}; do
      # Ahora usamos 'd' TAL CUAL, sin meter un 0. extra ni nada raro
      graph_file="${DATA_DIR}/erdos_n${n}_p0c${d}_${id}.graph"

      if [[ ! -f "${graph_file}" ]]; then
        echo "[WARN] No existe ${graph_file}, se omite."
        continue
      fi

      echo "[RUN] n=${n} d=${d} id=${id} -> ${graph_file}"

      # Ejecutamos BRKGA y guardamos TODA la salida en 'out'
      out=$("${EXE_POP}" -c "${CONF_FILE}" -t "${TIME_LIMIT}" -i "${graph_file}" 2>&1 || true)

      # Nos quedamos con la ÚLTIMA línea (respuesta;tiempo;estado_solucion)
      last_line=$(echo "${out}" | tail -n 1 | tr -d '\r')

      echo "  [RAW] ${last_line}"

      if [[ -z "${last_line}" ]]; then
        echo "[WARN] Salida vacía para ${graph_file}: ${out//$'\n'/ | }" >&2
        continue
      fi

      # Añadimos columnas: cantidad_vertices;densidad (densidad EXACTA como en el nombre)
      echo "${last_line};${n};${d}" >> "${OUT_CSV}"
      echo "  [OK] línea guardada con n=${n}, densidad=${d}"
    done
  done
done

echo "[DONE] Experimentos BRKGA ${TIME_LIMIT}s completados. Resultados en ${OUT_CSV}"
