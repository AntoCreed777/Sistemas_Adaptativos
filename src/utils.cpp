#include <fstream>
#include <stdexcept>
#include "graph_matrix.h"
#include "graph_list.h"

namespace graph_creation {
    GraphMatrix read_graph(const std::string& root) {
        std::ifstream file(root);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + root);
        }

        int num_vertices;
        file >> num_vertices;  // número de vértices
        GraphMatrix g(num_vertices);

        int u, v;
        while (file >> u >> v) {
            g.add_edge(u, v);
        }

        return g;
    } 

    GraphList read_graph_adjacent(const std::string& root) {
        std::ifstream file(root);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + root);
        }

        int num_vertices;
        file >> num_vertices;
        GraphList g(num_vertices);   // usa tu clase basada en listas

        int v1, v2;
        while (file >> v1 >> v2) {
            g.add_edge(v1, v2);
        }
        return g;
    }
}