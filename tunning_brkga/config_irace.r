library(irace)
library(parallel)

n_cores <- 6

# ---------------------------
# Definir parámetros
# ---------------------------
digits <- 4L

parameters <- parametersNew(
  param_int("population_size",       100, 2000, label = ""),
  param_real("elite_percentage",     0.05, 0.4,  label = "", digits = digits),
  param_real("mutants_percentage",   0.05, 0.4,  label = "", digits = digits),
  param_int("num_elite_parents",     1, 5,       label = ""),
  param_int("total_parents",         2, 10,      label = ""),
  param_real("pr_minimum_distance",  0.05, 0.3,  label = "", digits = digits),
  param_real("alpha_block_size",     0.5, 1.5,   label = "", digits = digits),
  param_real("pr_percentage",        0.1, 1.0,   label = "", digits = digits),
  param_int("num_exchange_individuals", 1, 5,    label = ""),
  param_real("shaking_intensity_lower_bound", 0, 0.7, label = "", digits = digits),
  param_real("shaking_intensity_upper_bound", 0.3, 0.9, label = "", digits = digits),
  param_int("exchange_interval",     0, 50,      label = ""),
  param_int("ipr_interval",          0, 50,      label = ""),
  param_int("shake_interval",        20, 100,    label = ""),
  param_int("reset_interval",        50, 300,    label = ""),
  param_int("stall_offset",          100, 500,   label = ""),

  forbidden = "(num_elite_parents > total_parents - 1) |
               (shaking_intensity_lower_bound >= shaking_intensity_upper_bound)"
)

checkParameters(parameters)

# ---------------------------
# Definir escenario
# ---------------------------
scenario <- list(
  targetRunner = "./runner.sh",
  targetCmdline = "{instance} {seed} {targetRunnerArgs}",
  trainInstancesDir = "./instancias",
  logFile = "irace.log",
  maxExperiments = 1000,
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
