#pragma once
#include <vector>

#include "graph_matrix.h"

namespace greedy_random {
    std::vector<int> solve_misp(GraphMatrix& graph, float alpha = 0.2);
}
