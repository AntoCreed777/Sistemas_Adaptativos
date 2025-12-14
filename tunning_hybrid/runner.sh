#!/bin/bash
CONF_DIR=tmp_conf

# ---------------------------
# Recibir argumentos
# ---------------------------
instance_file=$1
seed=$2

population_size=$3
elite_percentage=$4
mutants_percentage=$5
num_elite_parents=$6
total_parents=$7
stall_offset=$8

pr_percentage=$9
pr_minimum_distance=${10}
num_exchange_individuals=${11}
alpha_block_size=${12}

reset_interval=${13}

cantidad_chr_optimizar=${14}
time_limit_optimizacion=${15}
porcentaje_tiempo_aplicar_opt=${16}

# ---------------------------
# Archivo de configuración temporal
# ---------------------------
mkdir -p ${CONF_DIR}

conf_file="${CONF_DIR}/tunning_brkga_${seed}_$$.conf"

cat > "$conf_file" <<EOL
population_size $population_size
elite_percentage $elite_percentage
mutants_percentage $mutants_percentage
num_elite_parents $num_elite_parents
total_parents $total_parents
stall_offset $stall_offset

pr_percentage $pr_percentage
pr_minimum_distance $pr_minimum_distance
num_exchange_individuals $num_exchange_individuals
alpha_block_size $alpha_block_size

reset_interval $reset_interval

bias_type LOGINVERSE
num_independent_populations 1
pr_number_pairs 0
pr_type DIRECT
pr_selection BESTSOLUTION
pr_distance_function_type KENDALLTAU
shaking_type SWAP

ipr_interval 0
exchange_interval 0
shake_interval 0

EOL

# Ejecutar el algoritmo
score=$(../HYBRID.out -i "$instance_file" -t 30 -c "$conf_file" --cantidad_chr_optimizar $cantidad_chr_optimizar --time_limit_optimizacion $time_limit_optimizacion --porcentaje_tiempo_aplicar_opt $porcentaje_tiempo_aplicar_opt| tail -n 1 | cut -d';' -f1)

# Borrar el archivo temporal
rm -f "$conf_file"

# Esto debido a que MISP busca maximizar
# pero IRACE minimiza
echo $((-1 * score))
