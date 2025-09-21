#!/usr/bin/env bash
# Ejecuta Greedy.out sobre instancias Erdos y guarda resultados en CSV.
# Estructura esperada del proyecto:
#   - ./grafos/erdos_n<n>_p0c<densidad>_<id>.graph
#   - ./Greedy.out (ya compilado con make)

set -Eeuo pipefail

# --- Configurables -----------------------------------------------------------
DATA_DIR="grafos"              # Carpeta donde están los .graph
OUT_CSV="experiments_greedy.csv"
EXE_NAME="Greedy.out"

# Crear CSV con encabezado
printf "respuesta,tiempo,densidad,vertices\n" > "${OUT_CSV}"

# Función para buscar archivo .graph
find_graph_file() {
  local n="$1"; local dens="$2"; local id="$3"
  local fname="erdos_n${n}_p0c${dens}_${id}.graph"
  local path="${DATA_DIR}/${fname}"
  if [[ -f "${path}" ]]; then
    echo "${path}"; return 0
  fi
  # Variante sin cero de relleno (0.80 -> 0.8)
  local dens_alt
  dens_alt=$(echo "${dens}" | sed -E 's/0$//')
  fname="erdos_n${n}_p0c${dens_alt}_${id}.graph"
  path="${DATA_DIR}/${fname}"
  if [[ -f "${path}" ]]; then
    echo "${path}"; return 0
  fi
  return 1
}

export LC_ALL=C
VERTICES=(1000 2000 3000)
mapfile -t DENSIDADES < <(awk 'BEGIN {for(i=5;i<=95;i+=5){printf "%.2f\n", i/100}}')
IDS=($(seq 1 30))

for n in "${VERTICES[@]}"; do
  for d in "${DENSIDADES[@]}"; do
    for id in "${IDS[@]}"; do
      if ! graph_file=$(find_graph_file "${n}" "${d}" "${id}"); then
        echo "[WARN] No se encontró: erdos_n${n}_p0c${d}_${id}.graph" >&2
        continue
      fi
      if [[ ! -x "./${EXE_NAME}" ]]; then
        echo "[ERROR] No se encuentra el ejecutable './${EXE_NAME}'. Compílalo con 'make' antes de correr este script." >&2
        exit 1
      fi
      out=$("./${EXE_NAME}" -i "${graph_file}" 2>&1 || true)
      read -r respuesta tiempo < <(echo "${out}" | grep -Eo '[0-9]+(\.[0-9]+)?' | head -n 2 | paste -sd ' ' -)
      if [[ -z "${respuesta:-}" || -z "${tiempo:-}" ]]; then
        echo "[WARN] Salida inesperada para ${graph_file}: '${out//$'\n'/ | }'" >&2
        continue
      fi
      printf "%s,%s,%s,%s\n" "${respuesta}" "${tiempo}" "${d}" "${n}" >> "${OUT_CSV}"
      echo "[OK] n=${n} d=${d} id=${id} -> resp=${respuesta}, t=${tiempo}"
    done
  done
done

echo "[DONE] Experimentos completados. Resultados en ${OUT_CSV}"
