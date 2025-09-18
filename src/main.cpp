
#include <iostream>
#include "graph_matrix.h"
#include "utils.h"   // el header donde declaraste read_graph
#include "greedy_random.h"
#include "greedy.h"
#include "definiciones.h"

int main() {
    try {
        // Carga del grafo
        std::string base_root = "./src/grafos/";
        std::string file_name = "erdos_n1000_p0c0.1_1.graph";
        std::string root = base_root + file_name; // cámbiala por tu archivo real
        GraphMatrix g = graph_creation::read_graph(root);
        GraphMatrix g_copy = g;

        // Soluciona el grafo
        //auto solve = greedy_random::solve_misp(g);
        auto solve = greedy::solve_misp(g);

        // Impresion de resultado

        std::cout << "Vectores solucion de MISP:" << std::endl;

        for (auto vertex_of_solve: solve) {
            std::cout << vertex_of_solve << " ";
        }
        std::cout << std::endl;
        std::cout << "Cantidad de vectores: " << solve.size() << std::endl;
        
        if (graph_creation::check_consistency(g_copy, solve)){
            std::cout << VERDE;
            std::cout << "La respuesta es consistente" << std::endl;
            std::cout << RESET_COLOR;
        }
        else{
            std::cout << ROJO;
            std::cout << "La solucion no es consistente" << std::endl;
            std::cout << RESET_COLOR;
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
