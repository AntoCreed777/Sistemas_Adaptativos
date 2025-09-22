#!/usr/bin/env bash
# Ejecuta Greedy-probabilista.out sobre instancias Erdos y guarda resultados en CSV.
# Mínimos cambios respecto a experiments_greedy.sh:
#  - Ejecutable: Greedy-probabilista.out
#  - Parámetros: -i <instancia> -l <lista> -a <alpha>
#  - CSV: experiments_probabilistic.csv con columnas extra: alpha,lista

set -Eeuo pipefail

# --- Configurables -----------------------------------------------------------
DATA_DIR="grafos"                          # Carpeta donde están los .graph
OUT_CSV="experiments_probabilistic.csv"   # Archivo CSV de salida
EXE_NAME="Greedy-probabilista.out"        # Ejecutable ya compilado

# Parámetros de la heurística probabilística
ALPHAS=(0.05 0.1 0.2 0.3 0.4)
LIST_SIZES=(-1 20 10 5)                   # Orden requerido

# Crear CSV con encabezado
printf "respuesta,tiempo,densidad,vertices,alpha,lista\n" > "${OUT_CSV}"

# Función para buscar archivo .graph (acepta 0.80 y 0.8)
find_graph_file() {
  local n="$1"; local dens="$2"; local id="$3"
  local fname_l="erdos_n${n}_p0c${dens}_${id}.graph"
  local path_l="${DATA_DIR}/${fname_l}"
  if [[ -f "${path_l}" ]]; then
    echo "${path_l}"; return 0
  fi
  # Variante corta (0.80 -> 0.8)
  local dens_short
  dens_short=$(echo "${dens}" | sed -E 's/0$//')
  local fname_s="erdos_n${n}_p0c${dens_short}_${id}.graph"
  local path_s="${DATA_DIR}/${fname_s}"
  if [[ -f "${path_s}" ]]; then
    echo "${path_s}"; return 0
  fi
  return 1
}

export LC_ALL=C
VERTICES=(1000 2000 3000)
DENSIDADES=(0.05 0.10 0.15 0.20 0.25 0.30 0.35 0.40 0.45 0.50 0.55 0.60 0.65 0.70 0.75 0.80 0.85 0.90 0.95)
IDS=($(seq 1 30))

# Verificación de binario
if [[ ! -x "./${EXE_NAME}" ]]; then
  echo "[ERROR] No se encuentra el ejecutable './${EXE_NAME}'. Compílalo con 'make'." >&2
  exit 1
fi

for n in "${VERTICES[@]}"; do
  for d in "${DENSIDADES[@]}"; do
    for id in "${IDS[@]}"; do
      if ! graph_file=$(find_graph_file "${n}" "${d}" "${id}"); then
        echo "[WARN] No se encontró: erdos_n${n}_p0c${d}_${id}.graph" >&2
        continue
      fi
      for alpha in "${ALPHAS[@]}"; do
        for k in "${LIST_SIZES[@]}"; do
          # Ejecutar Greedy-probabilista con formato correcto: -i <instancia> -l <lista> -a <alpha>
          out=$("./${EXE_NAME}" -i "${graph_file}" -l "${k}" -a "${alpha}" 2>&1 || true)
          # Extraer los dos primeros números de la salida (respuesta y tiempo)
          read -r respuesta tiempo < <(printf '%s' "${out}" | grep -Eo '[0-9]+(\.[0-9]+)?' | head -n 2 | paste -sd ' ' -)
          if [[ -z "${respuesta:-}" || -z "${tiempo:-}" ]]; then
            safe_out=$(printf '%s' "${out}" | tr '\n' ' | ' || true)
            echo "[WARN] Salida inesperada para ${graph_file} (a=${alpha}, l=${k}): '${safe_out}'" >&2
            continue
          fi
          d_short=$(echo "${d}" | sed -E 's/0$//')
          printf "%s,%s,%s,%s,%s,%s\n" "${respuesta}" "${tiempo}" "${d_short}" "${n}" "${alpha}" "${k}" >> "${OUT_CSV}"
          echo "[OK] n=${n} d=${d_short} id=${id} a=${alpha} l=${k} -> resp=${respuesta}, t=${tiempo}"
        done
      done
    done
  done
done

echo "[DONE] Experimentos probabilísticos completados. Resultados en ${OUT_CSV}"