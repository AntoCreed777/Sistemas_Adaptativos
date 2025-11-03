library(irace)
library(parallel)

n_cores <- 6

# ---------------------------
# Definir parámetros
# ---------------------------
parameters_text <- '
population_size "" i (100,2000)
elite_percentage "" r (0.05,0.4)
mutants_percentage "" r (0.05,0.4)
num_elite_parents "" i (1,5)
total_parents "" i (2,10)
pr_minimum_distance "" r (0.05,0.3)
alpha_block_size "" r (0.5,1.5)
pr_percentage "" r (0.1,1.0)
num_exchange_individuals "" i (1,5)
shaking_type "" i (0,2)
shaking_intensity_lower_bound "" r (0.1,0.5)
shaking_intensity_upper_bound "" r (0.3,0.9)
maximum_running_time "" i (60,600)
exchange_interval "" i (0,50)
ipr_interval "" i (0,50)
shake_interval "" i (20,100)
reset_interval "" i (50,300)
stall_offset "" i (100,500)
'

parameters <- readParameters(text = parameters_text)

parameters$conditions <- list(
  "num_elite_parents <= total_parents - 1",
  "shaking_intensity_lower_bound < shaking_intensity_upper_bound"
)

# ---------------------------
# Definir escenario
# ---------------------------
scenario <- list(
  targetRunner = "./runner.sh",
  targetCmdline = "{instance} {seed} {targetRunnerArgs}",
  trainInstancesDir = "./instancias",
  logFile = "irace.log",
  maxExperiments = 510,
  parameters = parameters,
  parallel = n_cores
)

# ---------------------------
# Ejecutar irace
# ---------------------------
result <- irace(scenario)

# ---------------------------
# Guardar los mejores parámetros
# ---------------------------
write.table(result$elite, "mejores_parametros.txt", row.names = FALSE)
