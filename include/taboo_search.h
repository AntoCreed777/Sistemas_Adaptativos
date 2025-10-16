#pragma once
#include <vector>

// Adelanto para evitar incluir el header completo aquí.
class GraphMatrix;

namespace meta_taboo {

    void local_search_tabu(
        std::vector<int>& current_solution,
        GraphMatrix& graph,
        int tabu_len,
        double max_seconds,
        long long &ms
    );

    void local_search(std::vector<int>& current_solution, GraphMatrix& graph);

    std::vector<int> taboo_search(GraphMatrix& graph, int length_taboo_list, int max_seconds, long long &ms);

} 

