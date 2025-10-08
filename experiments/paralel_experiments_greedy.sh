#!/usr/bin/env bash
# Ejecuta Greedy.out sobre instancias Erdos y guarda resultados en CSV.
# Paraleliza con 5 trabajos simultáneos (ajustable con $1).
# Formato esperado: erdos_n<n>_p0c<densidad>_<id>.graph (p.ej., erdos_n1000_p0c0.1_1.graph)

### ESTE ARCHIVO FUE DE PRUEBA, NO SE USO PARA LOS EXPERIMENTOS NI PARA LA GENERACION DE LOS .csv

set -Eeuo pipefail

# --- Configurables -----------------------------------------------------------
DATA_DIR="grafos"                # Carpeta donde están los .graph
OUT_CSV="experiments/results/experiments_greedy.csv" # Archivo CSV de salida
EXE_NAME="Greedy.out"            # Ejecutable ya compilado
JOBS=${1:-5}                      # Paralelismo (por defecto 5)

export LC_ALL=C

# Verificación de binario
if [[ ! -x "./${EXE_NAME}" ]]; then
  echo "[ERROR] No se encuentra ejecutable './${EXE_NAME}'. Compílalo con 'make'." >&2
  exit 1
fi

# Crear CSV con encabezado (limpia anterior si existe)
printf "respuesta,tiempo,densidad,vertices
" > "${OUT_CSV}"

# Rangos de parámetros
VERTICES=(1000 2000 3000)
DENSIDADES=(0.05 0.10 0.15 0.20 0.25 0.30 0.35 0.40 0.45 0.50 0.55 0.60 0.65 0.70 0.75 0.80 0.85 0.90 0.95)
IDS=($(seq 1 30))

# Normaliza densidades: si terminan en 0 (0.80 -> 0.8)
short_density() {
  sed -E 's/0$//' <<<"$1"
}

# Procesa una instancia: n d id
process_one() {
  local n="$1" d="$2" id="$3"
  local d_short
  d_short=$(short_density "$d")

  # Prefiere forma corta en el nombre (p.ej., 0.1); si no existe, prueba la larga (0.10)
  local f_short="${DATA_DIR}/erdos_n${n}_p0c${d_short}_${id}.graph"
  local f_long="${DATA_DIR}/erdos_n${n}_p0c${d}_${id}.graph"
  local graph_file=""
  if [[ -f "$f_short" ]]; then
    graph_file="$f_short"
  elif [[ -f "$f_long" ]]; then
    graph_file="$f_long"
  else
    printf "[WARN] No se encontró: erdos_n%s_p0c%s_%s.graph
" "$n" "$d_short" "$id" >&2
    return 0
  fi

  # Ejecutar Greedy.out con -i <archivo>
  local out
  out=$("./${EXE_NAME}" -i "$graph_file" 2>&1 || true)

  # Extraer los dos primeros números (respuesta, tiempo)
  local respuesta tiempo
  read -r respuesta tiempo < <(printf '%s' "$out" | grep -Eo '[0-9]+(\.[0-9]+)?' | head -n 2 | paste -sd ' ' -)

  if [[ -z "${respuesta:-}" || -z "${tiempo:-}" ]]; then
    local safe_out
    safe_out=$(printf '%s' "$out" | tr '
' ' | ' || true)
    printf "[WARN] Salida inesperada para %s: '%s'
" "$graph_file" "${safe_out:-}" >&2
    return 0
  fi

  # Escribir línea al CSV con bloqueo para evitar carreras
  {
    flock -x 9
    printf "%s,%s,%s,%s
" "$respuesta" "$tiempo" "$d_short" "$n" >> "${OUT_CSV}"
  } 9>>"${OUT_CSV}"

  printf "[OK] n=%s d=%s id=%s -> resp=%s, t=%s
" "$n" "$d_short" "$id" "$respuesta" "$tiempo"
}

# Lanza trabajos en paralelo controlando concurrencia
for n in "${VERTICES[@]}"; do
  for d in "${DENSIDADES[@]}"; do
    for id in "${IDS[@]}"; do
      process_one "$n" "$d" "$id" &
      # Limitar número de jobs en background
      while (( $(jobs -p | wc -l) >= JOBS )); do sleep 0.1; done
    done
  done
done

# Esperar todos los trabajos
wait

printf "[DONE] Experimentos completados en paralelo (j=%s). Resultados en %s
" "${JOBS}" "${OUT_CSV}"
