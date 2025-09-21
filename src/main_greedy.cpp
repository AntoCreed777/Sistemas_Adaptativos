// greedy_main.cpp
// Uso: Greedy -i <instancia-problema>
// Salida: "valor_objetivo tiempo_ms"\n
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "cxxopts/cxxopts.hpp"
#include "graph_matrix.h"
#include "utils.h"
#include "greedy.h"

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("Greedy", "Greedy determinista para MISP (salida: valor tiempo_ms)");
        options.add_options()
            ("i,instancia", "Ruta a la instancia del problema (.graph)", cxxopts::value<std::string>())
            ("h,help", "Mostrar ayuda");

        auto result = options.parse(argc, argv);
        if (result.count("help") || !result.count("instancia")) {
            std::cerr << options.help() << "\n";
            return result.count("help") ? 0 : 1;
        }

        const std::string instancia = result["instancia"].as<std::string>();
        GraphMatrix g = graph_creation::read_graph(instancia);

        auto t0 = Clock::now();
        std::vector<int> sol = greedy::solve_misp(g);
        auto t1 = Clock::now();

        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << sol.size() << ';' << ms << '\n';
        return 0;
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error en opciones: " << e.what() << "\n"; return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n"; return 1;
    }
}