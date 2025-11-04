#!/bin/bash

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
pr_minimum_distance=$8
alpha_block_size=$9
pr_percentage=${10}
num_exchange_individuals=${11}
shaking_type=${12}
shaking_intensity_lower_bound=${13}
shaking_intensity_upper_bound=${14}
maximum_running_time=${15}
exchange_interval=${16}
ipr_interval=${17}
shake_interval=${18}
reset_interval=${19}
stall_offset=${20}

# ---------------------------
# Archivo de configuración temporal
# ---------------------------
conf_file="tunning_brkga.conf"

cat > "$conf_file" <<EOL
population_size $population_size
elite_percentage $elite_percentage
mutants_percentage $mutants_percentage
num_elite_parents $num_elite_parents
total_parents $total_parents
pr_minimum_distance $pr_minimum_distance
alpha_block_size $alpha_block_size
pr_percentage $pr_percentage
num_exchange_individuals $num_exchange_individuals
shaking_type $shaking_type
shaking_intensity_lower_bound $shaking_intensity_lower_bound
shaking_intensity_upper_bound $shaking_intensity_upper_bound
exchange_interval $exchange_interval
ipr_interval $ipr_interval
shake_interval $shake_interval
reset_interval $reset_interval
stall_offset $stall_offset

bias_type LOGINVERSE
num_independent_populations 1
pr_number_pairs 0
pr_type DIRECT
pr_selection BESTSOLUTION
pr_distance_function_type KENDALLTAU

EOL

# Ejecutar el algoritmo
score=$(../BRKGA.out -i "$instance_file" -t "$maximum_running_time" -c "$conf_file" | tail -n 1)

# Esto debido a que MISP busca maximizar
# pero IRACE minimiza
echo $((-1 * score))