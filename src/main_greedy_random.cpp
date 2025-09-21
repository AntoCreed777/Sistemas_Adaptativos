// greedy_prob_main.cpp
// Uso: Greedy-probabilista -i <instancia-problema> [-l <longitud-lista>] [-a <alpha>]
//   -l: longitud de la lista candidata (L). Usa -1 para infinita. (default: -1)
//   -a: alpha en [0,1] (default: 0.5)
// Salida: "valor_objetivo tiempo_ms"\n
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "cxxopts/cxxopts.hpp"
#include "graph_matrix.h"
#include "utils.h"
#include "greedy_random.h"

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("Greedy-probabilista", "Greedy aleatorizado para MISP (salida: valor tiempo_ms)");
        options.add_options()
            ("i,instancia", "Ruta a la instancia del problema (.graph)", cxxopts::value<std::string>())
            ("l,longitud", "Longitud de la lista candidata (L). -1=infinita", cxxopts::value<int>()->default_value("-1"))
            ("a,alpha", "Alpha en [0,1]", cxxopts::value<double>()->default_value("0.5"))
            ("h,help", "Mostrar ayuda");

        auto result = options.parse(argc, argv);
        if (result.count("help") || !result.count("instancia")) {
            std::cerr << options.help() << "\n";
            return result.count("help") ? 0 : 1;
        }

        const std::string instancia = result["instancia"].as<std::string>();
        int L = result["longitud"].as<int>();
        double alpha = result["alpha"].as<double>();

        GraphMatrix g = graph_creation::read_graph(instancia);

        auto t0 = Clock::now();
        std::vector<int> sol = greedy_random::solve_misp(g, alpha, L);
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