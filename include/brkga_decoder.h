#pragma once
#include <vector>
#include "graph_matrix.h"  // tu clase de grafo

#include "brkga_mp_ipr_cpp/brkga_mp_ipr/fitness_type.hpp"
#include "brkga_mp_ipr_cpp/brkga_mp_ipr/chromosome.hpp"

class BRKGADecoder {
public:
    BRKGADecoder(const GraphMatrix& g) : graph(g) {}

    BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool rewrite) {
        const unsigned n = chromosome.size();
        std::vector<bool> in_solution(n, false);

        // Orden de nodos según el valor del gen (descendente)
        std::vector<unsigned> idx(n);
        for (unsigned i = 0; i < n; ++i) idx[i] = i;

        std::sort(idx.begin(), idx.end(),
                  [&](unsigned a, unsigned b){ return chromosome[a] > chromosome[b]; });

        BRKGA::fitness_t fitness = 0;

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
                fitness++;
            }
        }

        return fitness;
    }

private:
    const GraphMatrix& graph;
};
