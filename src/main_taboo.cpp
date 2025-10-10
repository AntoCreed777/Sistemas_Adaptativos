// main_taboo.cpp
// Uso: Taboo -i <instancia-problema> [-L <tabu_len>] [-s <segundos>]
// Salida: "|S|;tiempo_ms"\n

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "cxxopts/cxxopts.hpp"
#include "graph_matrix.h"
#include "utils.h"          // para graph_creation::read_graph(...)
#include "taboo_search.h"   // tu código con namespace meta_taboo

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("Taboo", "Metaheuristica Tabu para MIS");
        options.add_options()
            ("i,input",   "Ruta de la instancia", cxxopts::value<std::string>())
            ("L,tabu_len","Largo de la lista tabu (default: max(5, n/100))", cxxopts::value<int>()->default_value("0"))
            ("s,seconds", "Tiempo maximo en segundos (default: 3)", cxxopts::value<int>()->default_value("3"))
            ("h,help",    "Mostrar ayuda");

        auto result = options.parse(argc, argv);

        if (result.count("help") || !result.count("input")) {
            std::cout << options.help() << "\n";
            return result.count("help") ? 0 : 1;
        }

        const std::string instancia = result["input"].as<std::string>();
        int tabu_len  = result["tabu_len"].as<int>();   // si 0 => se calcula luego
        int seconds   = result["seconds"].as<int>();
        if (seconds <= 0) seconds = 1;

        // Cargar grafo (mismo helper que en tu main_greedy)
        GraphMatrix g = graph_creation::read_graph(instancia);

        // Si no se especificó L, usar heurística por n
        if (tabu_len <= 0) {
            int n = g.get_num_vertices();
            tabu_len = std::max(5, n / 100);
        }

        auto t0 = Clock::now();
        std::vector<int> sol = meta_taboo::taboo_search(g, tabu_len, seconds);
        auto t1 = Clock::now();

        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        // MISMA salida que tu greedy_main: tamaño;milisegundos
        std::cout << sol.size() << ';' << ms << '\n';
        return 0;

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error en opciones: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
