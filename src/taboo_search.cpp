#include <vector>
#include <deque>
#include <chrono>
#include <unordered_set>
#include "greedy_random.h"
#include "graph_matrix.h"
#include "taboo_search.h"

namespace meta_taboo{

   static inline bool can_add_fast(int v,
                                    const std::vector<int>& S,
                                    const std::vector<char>& inS,
                                    const GraphMatrix& g) {
        if (v < 0 || v >= g.get_num_vertices()) return false;
        if (inS[v]) return false;
        for (int u : S) if (g.has_edge(u, v)) return false;
        return true;
    }

    // Rellena con 1-0 encadenados (idéntico espíritu a tu local_search)
    static void greedy_fill_10(std::vector<int>& S,
                               std::vector<char>& inS,
                               const GraphMatrix& g) {
        const int n = g.get_num_vertices();
        bool grew = true;
        while (grew) {
            grew = false;
            for (int v = 0; v < n; ++v) {
                if (!inS[v] && can_add_fast(v, S, inS, g)) {
                    S.push_back(v);
                    inS[v] = true;
                    grew = true;
                    break; // permitir encadenar y reiniciar
                }
            }
        }
    }

    // Búsqueda local con memoria Tabu (1-0 + 1-1, con prohibición de remover vértices recién agregados)
    // - tabu_len: tamaño de la lista tabu (tenencia)
    // - max_seconds: tope duro de tiempo
    void local_search_tabu(std::vector<int>& current_solution,
                           GraphMatrix& graph,
                           int tabu_len,
                           double max_seconds)
    {
        const int n = graph.get_num_vertices();

        // Estado inicial: asegurar pertenencia rápida y maximalidad por 1-0
        std::vector<char> inS(n, false);
        inS.shrink_to_fit();
        for (int u : current_solution) if (u >= 0 && u < n) inS[u] = true;
        greedy_fill_10(current_solution, inS, graph);

        std::vector<int> best = current_solution; // mejor histórico

        // Lista tabu: prohibimos REMOVER vértices recientemente AGREGADOS
        std::deque<int> tabu_fifo;
        std::unordered_set<int> tabu_remove;

        auto push_tabu = [&](int v_added) {
            // v_added pasa a estar prohibido para removerlo durante 'tabu_len' pasos
            tabu_fifo.push_back(v_added);
            tabu_remove.insert(v_added);
            if ((int)tabu_fifo.size() > tabu_len) {
                int old = tabu_fifo.front(); tabu_fifo.pop_front();
                tabu_remove.erase(old);
            }
        };

        auto start = std::chrono::steady_clock::now();

        while (true) {
            // Control de tiempo
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            if (elapsed >= max_seconds) break;

            // Intento 1-0 directo (crecer si es posible)
            bool grew = false;
            for (int v = 0; v < n; ++v) {
                if (!inS[v] && can_add_fast(v, current_solution, inS, graph)) {
                    current_solution.push_back(v);
                    inS[v] = true;
                    push_tabu(v); // lo acabamos de agregar; evita sacarlo de inmediato
                    grew = true;
                    if ((int)current_solution.size() > (int)best.size()) best = current_solution;
                    break; // reiniciar ciclo
                }
            }
            if (grew) continue;

            // Si no crecimos por 1-0, probamos 1-1 con aspiración
            int chosen_u = -1, chosen_v = -1;
            int best_neighbor_size = -1;
            std::vector<int> best_neighbor;
            std::vector<char> best_inS;

            for (int v = 0; v < n; ++v) {
                if (inS[v]) continue;

                // Contar conflictos de v con S
                int conflicts = 0, conflict_u = -1;
                for (int u : current_solution) {
                    if (graph.has_edge(u, v)) {
                        ++conflicts; conflict_u = u;
                        if (conflicts > 1) break;
                    }
                }
                if (conflicts != 1) continue; // 1-1 requiere exactamente un conflicto

                // Regla Tabu: prohibido remover un vértice recientemente agregado (salvo aspiración)
                bool is_tabu = (tabu_remove.find(conflict_u) != tabu_remove.end());

                // Construimos vecino: swap u -> v
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

                // Rellenar con 1-0 (posibles desbloqueos tras el swap)
                greedy_fill_10(cand, inCand, graph);

                int cand_size = (int)cand.size();
                bool aspiration = (cand_size > (int)best.size());

                if (!is_tabu || aspiration) {
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

            // Aceptar mejor vecino y actualizar Tabu (marcamos v como "no remover")
            current_solution = std::move(best_neighbor);
            inS = std::move(best_inS);
            push_tabu(chosen_v);

            if ((int)current_solution.size() > (int)best.size()) {
                best = current_solution;
            }
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
        in_solution.shrink_to_fit(); // no "raro"; sólo evitar capacidad extra
        for (int u : current_solution) {
            if (u >= 0 && u < n) in_solution[u] = true;
        }

        auto can_add = [&](int v) -> bool {
            if (v < 0 || v >= n) return false;
            if (in_solution[v]) return false;
            for (int u : current_solution) {
                if (graph.has_edge(u, v)) return false;
            }
            return true;
        };

        bool improved = true;
        while (improved) {
            improved = false;

            // --- Movimiento 1-0: intentar agregar un vértice factible ---
            for (int v = 0; v < n; ++v) {
                if (!in_solution[v] && can_add(v)) {
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
        GraphMatrix copy_graph = graph;

        std::vector<int> initial_solution =  greedy_random::solve_misp(copy_graph);
        std::deque<int> taboo_next_elimination(length_taboo_list, -1);
        std::unordered_set<int> taboo_list;


        auto start = std::chrono::steady_clock::now();

        while (true) {
            auto now = std::chrono::steady_clock::now();
            double elapsed_seconds = std::chrono::duration<double>(now - start).count();

            if (elapsed_seconds >= max_seconds)
                break;

            local_search_tabu(initial_solution, graph, /*tabu_len=*/std::max(5, graph.get_num_vertices()/100),
                              /*max_seconds=*/max_seconds);
        }

        // Por ahora devolvemos la solución inicial (ajusta al final)
        return initial_solution;
    }
}
