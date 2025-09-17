
#include <iostream>
#include "graph_matrix.h"
#include "utils.h"   // el header donde declaraste read_graph
#include "greedy.h"

int main() {
    try {
        // Carga del grafo
        std::string ruta = "erdos_n3000_p0c0.8_8.graph"; // cámbiala por tu archivo real
        GraphMatrix g = graph_creation::read_graph(ruta);

        // Soluciona el grafo
        auto solve = solve_misp(g);

        // Impresion de resultado

        std::cout << "Vectores solucion de MISP:" << std::endl;

        for (auto vertex_of_solve: solve) {
            std::cout << vertex_of_solve << " ";
        }
        std::cout << std::endl;
        std::cout << "Cantidad de vectores: " << solve.size() << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
