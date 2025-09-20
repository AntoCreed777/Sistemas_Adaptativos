#include "greedy.h"
#include <limits>

#define INF std::numeric_limits<int>::max()


namespace greedy {
    std::vector<int> get_vertices_with_min_degree(GraphMatrix& graph) {
        int num_vertex = graph.get_num_vertices();
        int min_degree = INF;
        std::vector<int> vertices_with_min_degree;

        for (auto i = 0; i < num_vertex; i++) {
            int degree = graph.get_degree(i);

            if (degree < 0) continue;   // Esto por la eliminacion de forma logica del vertice

            if (degree < min_degree) {
                min_degree = degree;
                vertices_with_min_degree.clear();
                vertices_with_min_degree.push_back(i);
            }
            else if (degree == min_degree) {
                vertices_with_min_degree.push_back(i);
            }
        }

        return vertices_with_min_degree;
    }

    int choose_vertex_from_list_of_vertices_min_degree(std::vector<int>& vertices_with_min_degree) {
        // Si cambia la politica de eleccion solo se debera de modificar aqui
        // Por eso cree esta funcion
        return vertices_with_min_degree[0];
    }


    std::vector<int> solve_misp(GraphMatrix& graph) {
        std::vector<int> solution_vertices;

        while(!graph.is_empty()) {
            // Eligo el vertice
            auto vertices_with_min_degree = get_vertices_with_min_degree(graph);
            int chosen_vertex = choose_vertex_from_list_of_vertices_min_degree(vertices_with_min_degree);

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
