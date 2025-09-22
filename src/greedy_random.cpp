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

                //// Selecciono aleatoriamente uno de los primeros 'random_select_list_length' vértices (o todos si hay menos)
                // Calcula el rango de selección inicial: los primeros 'selection_range' vértices ordenados por grado ascendente.
                int selection_range = std::min(random_select_list_length, static_cast<int>(remaining_vertices.size()));
                // Obtiene el grado del último vértice dentro del rango inicial.
                int degree_limit = graph.get_degree(remaining_vertices[selection_range - 1]);

                // Queremos incluir en la selección todos los vértices que tengan el mismo grado que el último del rango inicial,
                // aunque estén fuera de ese rango. Esto evita sesgos por el ordenamiento y asegura que todos los vértices
                // con el mismo grado mínimo tengan la misma probabilidad de ser elegidos.

                // range_start: inicio del vector ordenado
                auto range_start = remaining_vertices.begin();

                // range_limit: fin del rango inicial de selección
                auto range_limit = remaining_vertices.begin() + selection_range;

                // range_end: primer vértice cuyo grado es mayor que degree_limit (usando upper_bound)
                // Esto extiende el rango hasta incluir todos los vértices con degree == degree_limit
                auto range_end = std::upper_bound(
                    range_limit,
                    remaining_vertices.end(),
                    degree_limit,
                    [&graph](int value, int idx) {
                        // value es degree_limit, idx es el índice del vértice
                        // upper_bound busca el primer vértice con grado > degree_limit
                        return value < graph.get_degree(idx);
                    }
                );

                // El rango de selección final es la unión lógica de los primeros selection_range vértices
                // y todos los que siguen con el mismo grado (degree_limit)
                int pos_range_end = std::distance(range_start, range_end - 1);
                // Selecciona aleatoriamente uno de los vértices dentro del rango extendido
                std::uniform_int_distribution<> dis(0, pos_range_end);
                chosen_vertex = *(range_start + dis(gen));
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
    
    int solve_misp_size(GraphMatrix& graph){
        return solve_misp(graph).size();
    }

}
