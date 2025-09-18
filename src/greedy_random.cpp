#include "greedy_random.h"
#include "greedy.h"
#include <random>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "definiciones.h"

namespace greedy_random {
    std::vector<int> solve_misp(GraphMatrix& graph, float alpha, int random_select_list_length) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist_alpha(0.0, 1.0);

        std::vector<int> solution_vertices;

        if (random_select_list_length == -1) random_select_list_length = INF;

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

                if (remaining_vertices.empty()) {
                    throw std::runtime_error("No quedan vértices restantes para elegir aleatoriamente.");
                }

                // Ordeno los vértices restantes por grado ascendente
                std::sort(
                    remaining_vertices.begin(),
                    remaining_vertices.end(),
                    [&graph](int a, int b) {
                        return graph.get_degree(a) < graph.get_degree(b);
                    }
                );

                // Selecciono aleatoriamente uno de los primeros 'random_select_list_length' vértices (o todos si hay menos)
                int selection_range = std::min(random_select_list_length, static_cast<int>(remaining_vertices.size())) - 1;
                std::uniform_int_distribution<> dis(0, selection_range);
                chosen_vertex = remaining_vertices[dis(gen)];
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
