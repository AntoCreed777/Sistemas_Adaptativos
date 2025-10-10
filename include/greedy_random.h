#pragma once
#include <vector>

#include "graph_matrix.h"
#include "definiciones.h"

namespace greedy_random {
    std::vector<int> solve_misp(GraphMatrix& graph, float alpha = 0.05, int random_select_list_length = INF);
    int solve_misp_size(GraphMatrix& graph);
}
