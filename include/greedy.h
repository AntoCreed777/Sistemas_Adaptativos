#pragma once
#include <vector>

#include "graph_matrix.h"

namespace greedy {
    std::vector<int> get_vertices_with_min_degree(GraphMatrix& graph);

    int choose_vertex_from_list_of_vertices_min_degree(std::vector<int>& vertices_with_min_degree);

    std::vector<int> solve_misp(GraphMatrix& graph);

    int solve_misp_size(GraphMatrix& graph);

}
