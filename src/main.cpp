
#include <iostream>
#include "graph_matrix.h"
#include "utils.h"   // el header donde declaraste read_graph

int main() {
    try {
        std::string ruta = "erdos_n3000_p0c0.95_10.graph"; // cámbiala por tu archivo real
        GraphMatrix g = graph_creation::read_graph(ruta);

        std::cout << "Número de vértices: " << g.get_num_vertices() << "\n";
        std::cout << "Número de aristas:  " << g.get_num_edges() << "\n\n";

        // Mostrar las primeras 5 filas de vecinos (o menos si hay pocos vértices)
        int limit = std::min(5, g.get_num_vertices());
        for (int v = 0; v < limit; ++v) {
            std::cout << "Vecinos de " << v << ": ";
            auto vecinos = g.get_neighbors(v);
            for (int u : vecinos) {
                std::cout << u << " ";
            }
            std::cout << "\nHay " << g.degree(v) << " aristas\n";
            std::cout << "\n";
        }

        std::cout << "\nHay " << g.get_num_edges() << " aristas totales \n";
        std::cout << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
