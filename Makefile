# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS_BASE = -I ./include -I ./libs -Wall
CXXFLAGS_WARNINGS = -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wfloat-equal
CXXFLAGS_OPTIMIZATIONS = -O2 -march=native -mtune=native
CXXFLAGS_DEBUGGING = -g #-ggdb	# Descomentar para depurar con GDB

# Flags de compilación para diferentes configuraciones
CXXFLAGS_RELEASE = $(CXXFLAGS_BASE) $(CXXFLAGS_OPTIMIZATIONS) #$(CXXFLAGS_WARNINGS)
CXXFLAGS_DEBUG = $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUGGING) #$(CXXFLAGS_WARNINGS)

# Configuración de compilación
CXXFLAGS = $(CXXFLAGS_DEBUG)	 # Cambiar a $(CXXFLAGS_RELEASE) para compilación de producción

# Directorios y archivos
OBJ_DIR = build
TARGET = main.out
GREEDY = Greedy.out
GREEDY_PROB = Greedy-probabilista.out
TABOO = Taboo.out
BRKGA = BRKGA.out

# Identificar mains y fuentes comunes
MAINS = ./src/main.cpp ./src/main_greedy.cpp ./src/main_greedy_random.cpp ./src/main_taboo.cpp ./src/main_brkga.cpp
ALL_SOURCES = $(wildcard ./src/*.cpp)
COMMON_SOURCES = $(filter-out $(MAINS), $(ALL_SOURCES))

COMMON_OBJECTS = $(patsubst ./src/%.cpp, $(OBJ_DIR)/%.o, $(COMMON_SOURCES))
MAIN_OBJ = $(OBJ_DIR)/main.o
GREEDY_OBJ = $(OBJ_DIR)/main_greedy.o
GREEDY_PROB_OBJ = $(OBJ_DIR)/main_greedy_random.o
TABOO_OBJ = $(OBJ_DIR)/main_taboo.o

# Indica que las siguientes reglas no son archivos y deben ser ejecutadas desde 0 siempre
.PHONY: all clean run debug memoria

# Regla por defecto: compilar el programa principal y los extras
all: $(TARGET) $(GREEDY) $(GREEDY_PROB) $(TABOO) $(BRKGA)

# Ejecutable principal: usa main.cpp
$(TARGET): $(COMMON_OBJECTS) $(MAIN_OBJ) | $(OBJ_DIR)
	@echo "Compilando el programa..."
	@$(CXX) $(COMMON_OBJECTS) $(MAIN_OBJ) -o $(TARGET) $(CXXFLAGS)

# Ejecutable Greedy (determinista): usa main_greedy.cpp
$(GREEDY): $(COMMON_OBJECTS) $(GREEDY_OBJ) | $(OBJ_DIR)
	@echo "Compilando Greedy..."
	@$(CXX) $(COMMON_OBJECTS) $(GREEDY_OBJ) -o $(GREEDY) $(CXXFLAGS)

# Ejecutable Greedy-probabilista: usa main_greedy_random.cpp
$(GREEDY_PROB): $(COMMON_OBJECTS) $(GREEDY_PROB_OBJ) | $(OBJ_DIR)
	@echo "Compilando Greedy-probabilista..."
	@$(CXX) $(COMMON_OBJECTS) $(GREEDY_PROB_OBJ) -o $(GREEDY_PROB) $(CXXFLAGS)

# Ejecutable Tabu: usa main_taboo.cpp
$(TABOO): $(COMMON_OBJECTS) $(TABOO_OBJ) | $(OBJ_DIR)
	@echo "Compilando Tabu..."
	@$(CXX) $(COMMON_OBJECTS) $(TABOO_OBJ) -o $(TABOO) $(CXXFLAGS)

# Ejecutable Tabu: usa main_brkga.cpp
$(BRKGA): 
	@echo "Compilando BRKGA..."
	@$(CXX) -o $(BRKGA) $(CXXFLAGS) src/main_brkga.cpp src/utils.cpp src/graph_matrix.cpp src/graph_list.cpp -I ./libs/brkga_mp_ipr_cpp/brkga_mp_ipr -std=c++20

# Regla para compilar los archivos objeto y guardarlos en build/
$(OBJ_DIR)/%.o: ./src/%.cpp | $(OBJ_DIR)
	@echo "Compilando $<..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# Crear el directorio build si no existe
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Regla para ejecutar el programa principal (se mantiene igual)
run: $(TARGET)
	@echo "Ejecutando el programa...\n"
	@./$(TARGET)

# Regla para debuggear el programa principal
debug: $(TARGET)
	@gdb ./$(TARGET)

# Regla para investigar las fugas de memoria (programa principal)
memoria: $(TARGET)
	@valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Regla para limpiar los archivos generados
clean:
	@rm -rf $(OBJ_DIR) $(TARGET) $(GREEDY) $(GREEDY_PROB) $(TABOO) $(BRKGA)
