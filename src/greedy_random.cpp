#include "greedy_random.h"
#include "greedy.h"
#include <random>
#include <stdexcept>
#include <iostream>

namespace greedy_random {
    std::vector<int> solve_misp(GraphMatrix& graph, float alpha) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist_alpha(0.0, 1.0);

        std::vector<int> solution_vertices;

        while(!graph.is_empty()) {
            int chosen_vertex;
            if (dist_alpha(gen) < alpha) {
                // Eleccion aleatoria dentro de los vertices restantes
                // Busco los vertices restantes
                int num_vertex = graph.get_num_vertices();
                std::vector<int> remaining_vertices;
                for (int i = 0; i < num_vertex; ++i) {
                    if (graph.get_degree(i) >= 0) {
                        remaining_vertices.push_back(i);
                    }
                }
                // Eligo de entre los vertices restantes
                if (!remaining_vertices.empty()) {
                    std::uniform_int_distribution<> dis(0, remaining_vertices.size() - 1);
                    chosen_vertex = remaining_vertices[dis(gen)];
                }
                else {
                    throw std::runtime_error("No quedan vértices restantes para elegir aleatoriamente.");
                }
            }
            else {
                // Eleccion segun heuristica
                auto vertices_with_min_degree = greedy::get_vertices_with_min_degree(graph);
                chosen_vertex = greedy::choose_vertex_from_list_of_vertices_min_degree(vertices_with_min_degree);
            }
            // Guardo el vertice en los vertices de solucion
            solution_vertices.push_back(chosen_vertex);
    
            // Elimino el vertice y sus vecinos del grafo
            graph.remove_neighbors_of_vertex_and_this_vertex(chosen_vertex);
        }

        return solution_vertices;
    }
}
