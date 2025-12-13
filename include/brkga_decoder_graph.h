#pragma once
#include <vector>
#include <algorithm>  // std::sort
#include <numeric>    // std::iota
#include "graph_matrix.h"

#include "brkga_mp_ipr_cpp/brkga_mp_ipr/chromosome.hpp"

class BRKGADecoderGraph {
public:
    BRKGADecoderGraph(const GraphMatrix& g) : graph(g) {}

    std::vector<int> decode(BRKGA::Chromosome& chromosome) {
        const unsigned n = chromosome.size();
        std::vector<bool> in_solution(n, false);

        // Orden de nodos según el valor del gen (descendente)
        std::vector<unsigned> idx(n);
        std::iota(idx.begin(), idx.end(), 0);   // Rellena con 0, 1, 2, ...

        std::sort(idx.begin(), idx.end(),
                  [&](unsigned a, unsigned b){ return chromosome[a] > chromosome[b]; });

        for (unsigned node : idx) {
            bool can_add = true;
            for (unsigned i = 0; i < n; ++i) {
                if (in_solution[i] && graph.has_edge(node, i)) {
                    can_add = false;
                    break;
                }
            }
            if (can_add) {
                in_solution[node] = true;
            }
        }

        std::vector<int> solution;

        for (int i=0; i< graph.get_num_vertices(); i++) {
            if (in_solution[i]) solution.push_back(i);
        }

        return solution;
    }

private:
    const GraphMatrix& graph;
};
