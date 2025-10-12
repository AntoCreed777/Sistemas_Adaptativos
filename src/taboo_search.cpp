#include <vector>
#include <deque>
#include <chrono>
#include <unordered_set>
#include "greedy.h"
#include "taboo_search.h"
#include "greedy_random.h"
#include "graph_matrix.h"


namespace meta_taboo{

    /**
     * Valida que el vertice sea valido: 0 <= v < g.get_num_vertices(),
     * si es que esta en la solucion actual: inS[v],
     * y si esta conectado con algun vertice que ya este en la solucion.
     */
    static inline bool can_add (
        int v,
        const std::vector<int>& S,
        const std::vector<char>& inS,
        const GraphMatrix& g
    ) {
        if (v < 0 || v >= g.get_num_vertices() || inS[v]) return false;
        for (int u : S) if (g.has_edge(u, v)) return false;
        return true;
    }

    /**
     * Hasta que queden vertices que se puedan agregar a la solucion permanece en bucle recorriendo los
     * vertices y validando si se pueden agregar o no.
     */
    static void greedy_fill_10(
        std::vector<int>& S,
        std::vector<char>& inS,
        const GraphMatrix& g
    ) {
        const int n = g.get_num_vertices();
        bool grew = true;
        while (grew) {
            grew = false;
            for (int v = 0; v < n; ++v) {
                if (can_add(v, S, inS, g)) {
                    S.push_back(v);
                    inS[v] = true;
                    grew = true;
                    break; // permitir encadenar y reiniciar
                }
            }
        }
    }

    // Búsqueda local con memoria Tabu (1-0 + 1-1, con prohibición simétrica de agregados/removidos)
    // - tabu_len: tamaño de la lista tabu (tenencia)
    // - max_seconds: tope duro de tiempo (deadline interno)
    void local_search_tabu(
        std::vector<int>& current_solution,
        GraphMatrix& graph,
        int tabu_len,
        double max_seconds
    ) {
        const int n = graph.get_num_vertices();

        // Estado inicial: asegurar pertenencia rápida y maximalidad por 1-0
        std::vector<char> inS(n, false);
        for (int u : current_solution) {
            if (u >= 0 && u < n) inS[u] = true;
        }
        greedy_fill_10(current_solution, inS, graph);

        std::vector<int> best = current_solution; // mejor histórico

        // Tabu simétrico: no remover los recién agregados, ni re-agregar los recién removidos
        std::deque<int> tabu_added_fifo, tabu_removed_fifo;
        std::unordered_set<int> tabu_cant_remove; // vértices recién agregados
        std::unordered_set<int> tabu_cant_add;    // vértices recién removidos

        auto push_tabu_add = [&](int v_added) {
            tabu_added_fifo.push_back(v_added);
            tabu_cant_remove.insert(v_added);
            if ((int)tabu_added_fifo.size() > tabu_len) {
                int old = tabu_added_fifo.front(); tabu_added_fifo.pop_front();
                tabu_cant_remove.erase(old);
            }
        };
        auto push_tabu_remove = [&](int v_removed) {
            tabu_removed_fifo.push_back(v_removed);
            tabu_cant_add.insert(v_removed);
            if ((int)tabu_removed_fifo.size() > tabu_len) {
                int old = tabu_removed_fifo.front(); tabu_removed_fifo.pop_front();
                tabu_cant_add.erase(old);
            }
        };

        // Deadline único interno
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::duration<double>(max_seconds);
        auto time_up = [&](){ return std::chrono::steady_clock::now() >= deadline; };

        while (!time_up()) {
            // Intento 1-0 directo (crecer si es posible)
            bool grew = false;
            for (int v = 0; v < n; ++v) {
                if (!can_add(v, current_solution, inS, graph)) continue;
                // Regla Tabu: no re-agregar recién removidos, salvo aspiración por BEST
                bool is_tabu_add = (tabu_cant_add.find(v) != tabu_cant_add.end());
                bool allow_by_asp = ((int)current_solution.size() + 1 > (int)best.size());
                if (is_tabu_add && !allow_by_asp) continue;

                current_solution.push_back(v);
                inS[v] = true;
                push_tabu_add(v);
                grew = true;
                if ((int)current_solution.size() > (int)best.size()) best = current_solution;
                break; // reiniciar ciclo
            }
            if (grew) continue;

            // Si no crecimos por 1-0, probamos 1-1 con aspiración (mejora incumbente o best)
            int chosen_u = -1, chosen_v = -1;
            int best_neighbor_size = -1;
            std::vector<int> best_neighbor;
            std::vector<char> best_inS;

            for (int v = 0; v < n; ++v) {
                if (inS[v]) continue; // sólo para v fuera de S

                // Contar conflictos de v con S
                int conflicts = 0, conflict_u = -1;
                for (int u : current_solution) {
                    if (graph.has_edge(u, v)) {
                        ++conflicts; conflict_u = u;
                        if (conflicts > 1) break; // 1-1 requiere exactamente un conflicto
                    }
                }
                if (conflicts != 1) continue;

                // Reglas Tabu
                bool tabu_on_remove = (tabu_cant_remove.find(conflict_u) != tabu_cant_remove.end());
                bool tabu_on_add    = (tabu_cant_add.find(v)        != tabu_cant_add.end());

                // Construimos vecino: swap u -> v y rellenamos 1-0
                std::vector<int> cand = current_solution;
                std::vector<char> inCand = inS;

                // eliminar conflict_u
                for (size_t i = 0; i < cand.size(); ++i) {
                    if (cand[i] == conflict_u) { cand[i] = cand.back(); cand.pop_back(); break; }
                }
                inCand[conflict_u] = false;
                // agregar v
                cand.push_back(v);
                inCand[v] = true;

                greedy_fill_10(cand, inCand, graph);

                int cand_size = (int)cand.size();
                bool improves_incumbent = cand_size > (int)current_solution.size();
                bool improves_best      = cand_size > (int)best.size();
                bool allow_by_aspiration = improves_incumbent || improves_best;

                if (!(tabu_on_remove || tabu_on_add) || allow_by_aspiration) {
                    if (cand_size > best_neighbor_size) {
                        best_neighbor_size = cand_size;
                        best_neighbor = std::move(cand);
                        best_inS = std::move(inCand);
                        chosen_u = conflict_u;
                        chosen_v = v;
                    }
                }
            }

            // Si no encontramos swap válido, estamos estancados: terminar
            if (chosen_u == -1) break;

            // Aceptar mejor vecino y actualizar Tabu (marcamos v como "no remover" y u como "no re-agregar")
            current_solution = std::move(best_neighbor);
            inS = std::move(best_inS);
            push_tabu_add(chosen_v);
            push_tabu_remove(chosen_u);

            if ((int)current_solution.size() > (int)best.size()) best = current_solution;
        }

        // Devolver el mejor histórico alcanzado
        current_solution = std::move(best);
    }


    // Implementa LS con movimientos 1-0 y 1-1.
    // Cambios mínimos: pasar current_solution por referencia para que la LS tenga efecto.
    void local_search(std::vector<int>& current_solution, GraphMatrix& graph){
        const int n = graph.get_num_vertices();

        // Marcador rápido de pertenencia a la solución
        std::vector<char> in_solution(n, false);
        for (int u : current_solution) {
            if (u >= 0 && u < n) in_solution[u] = true;
        }

        bool improved = true;
        while (improved) {
            improved = false;

            // --- Movimiento 1-0: intentar agregar un vértice factible ---
            for (int v = 0; v < n; ++v) {
                if (can_add(v, current_solution, in_solution, graph)) {
                    current_solution.push_back(v);
                    in_solution[v] = true;
                    improved = true;
                    // Tras una mejora, reiniciar búsqueda (permitir encadenar 1-0)
                    break;
                }
            }
            if (improved) continue;

            // --- Movimiento 1-1: swap {u in S} por {v not in S} con un único conflicto ---
            bool swapped = false;
            for (int v = 0; v < n && !swapped; ++v) {
                if (in_solution[v]) continue;

                int conflicts = 0;
                int conflict_u = -1;
                for (int u : current_solution) {
                    if (graph.has_edge(u, v)) {
                        ++conflicts;
                        conflict_u = u;
                        if (conflicts > 1) break; // sólo nos interesa exactamente un conflicto
                    }
                }

                if (conflicts == 1) {
                    // Quitar u
                    in_solution[conflict_u] = false;
                    for (size_t i = 0; i < current_solution.size(); ++i) {
                        if (current_solution[i] == conflict_u) {
                            current_solution[i] = current_solution.back();
                            current_solution.pop_back();
                            break;
                        }
                    }
                    // Agregar v
                    current_solution.push_back(v);
                    in_solution[v] = true;
                    swapped = true;
                    improved = true;
                    // No rompemos el MIS: v sólo chocaba con u, ya removido
                }
            }
            // El siguiente ciclo intentará otra vez 1-0; si no hay, buscará otro 1-1, etc.
        }
    }


    std::vector<int> taboo_search(GraphMatrix& graph, int length_taboo_list, int max_seconds){
        // Semilla inicial: greedy determinista (puedes cambiar a greedy_random si prefieres diversificar)
        std::vector<int> solution = greedy::solve_misp(graph);

        // Ejecutamos una sola corrida con deadline interno; usamos el largo Tabu entregado
        local_search_tabu(
            solution,
            graph,
            /*tabu_len=*/length_taboo_list,
            /*max_seconds=*/(double)max_seconds
        );

        return solution;
    }
}
