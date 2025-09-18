
#include <iostream>
#include "cxxopts.hpp"
#include "graph_matrix.h"
#include "utils.h"   // el header donde declaraste read_graph
#include "greedy_random.h"
#include "greedy.h"
#include "definiciones.h"
#include "constantes_opciones.h"

void print_help(const cxxopts::Options& options) {
    std::cout << MAGENTA << "===== MISP Solver =====" << RESET_COLOR << std::endl;
    std::cout << AMARILLO << "Flags obligatorias:" << RESET_COLOR << std::endl;
    std::cout << VERDE << "  -a, --algoritmo      " << DESC_ALGORITMO << RESET_COLOR << std::endl;
    std::cout << VERDE << "  -i, --instancia      " << DESC_INSTANCIA << RESET_COLOR << std::endl << std::endl;

    std::cout << CIAN << "Ejemplo de uso:" << RESET_COLOR << std::endl;
    std::cout << AZUL << "  ./main.out -a Greedy -i grafos/test.graph" << RESET_COLOR << std::endl << std::endl;

    std::cout << MAGENTA << "===== Todas las opciones =====" << RESET_COLOR << std::endl;
    std::cout << options.help() << std::endl;
}


int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("main.out", "Greedy y Greedy-random para solucionar \"Maximum Independent Set Problem\" (MISP).");

        options.add_options()
            (OPT_ALGORITMO_COMBINED, DESC_ALGORITMO, cxxopts::value<std::string>())
            (OPT_INSTANCIA_COMBINED, DESC_INSTANCIA, cxxopts::value<std::string>())
            (
                OPT_LONGITUD_LISTA_COMBINED,
                DESC_LONGITUD_LISTA,
                cxxopts::value<int>()->default_value(std::to_string(DEFAULT_LONGITUD_LISTA))
            )
            (
                OPT_ALPHA_COMBINED,
                DESC_ALPHA,
                cxxopts::value<double>()->default_value(std::to_string(DEFAULT_ALPHA))
            )
            (OPT_HELP_COMBINED, DESC_HELP);

        auto result = options.parse(argc, argv);

        if (result.count(OPT_HELP) || !result.count(OPT_ALGORITMO) || !result.count(OPT_INSTANCIA)) {
            print_help(options);
            return 0;
        }


        std::string algoritmo = result[OPT_ALGORITMO].as<std::string>();
        std::string instancia = result[OPT_INSTANCIA].as<std::string>();

        // Carga del grafo
        GraphMatrix g = graph_creation::read_graph(instancia);
        GraphMatrix g_copy = g;

        std::vector<int> solve;

        // Redireccion al algoritmo seleccionado
        if (algoritmo == GREEDY) {
            std::cout << "Ejecutando GREEDY sobre instancia: " << instancia << std::endl;
            solve = greedy::solve_misp(g);
        }
        else if (algoritmo == GREEDY_RANDOM) {
            int longitud = result[OPT_LONGITUD_LISTA].as<int>();
            double alpha = result[OPT_ALPHA].as<double>();

            std::cout << "Ejecutando GREEDY-RANDOM sobre instancia: " << instancia << std::endl;

            std::cout << " - Longitud lista: ";
            if (longitud == -1) std::cout << "Infinita" << std::endl;
            else std::cout << longitud << std::endl;

            std::cout << " - Alpha: " << alpha << std::endl;

            solve = greedy_random::solve_misp(g, alpha, longitud);
        }
        else {
            std::cerr << "Modo desconocido: " << algoritmo << std::endl;
            return 1;
        }

        // Muestra los resultados
        std::cout << "Vertices solucion de MISP:" << std::endl;

        for (auto vertex_of_solve: solve)
            std::cout << vertex_of_solve << " ";
        std::cout << std::endl;
        std::cout << "Cantidad de vertices: " << solve.size() << std::endl;
        
        // Validacion de que la respuesta sea valida (que ningun vertice este en la vecindad de los demas)
        if (graph_creation::check_consistency(g_copy, solve)) {
            std::cout << VERDE "La respuesta es consistente" RESET_COLOR << std::endl;
        }
        else {
            std::cout << ROJO "La solucion no es consistente" RESET_COLOR << std::endl;
        }
    }
    catch (const cxxopts::exceptions::exception& e) {
        std::cerr << ROJO << "Error en opciones: " << e.what() << RESET_COLOR << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << ROJO << "Error inesperado: " << e.what() << RESET_COLOR << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << ROJO << "Error desconocido." << RESET_COLOR << std::endl;
        return 1;
    }
    return 0;
}
