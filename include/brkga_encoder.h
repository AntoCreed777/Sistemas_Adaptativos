#pragma once
#include <vector>
#include <random>
#include <stdexcept>

#include "graph_matrix.h"
#include "brkga_mp_ipr_cpp/brkga_mp_ipr/chromosome.hpp"


/**
 * @brief Codifica una solución en un cromosoma de tipo BRKGA::Chromosome.
 *
 * Esta función toma una solución representada por un conjunto de nodos (índices enteros)
 * pertenecientes a un grafo y la transforma en un cromosoma con claves aleatorias (random keys),
 * según el esquema del algoritmo BRKGA (Biased Random-Key Genetic Algorithm).
 *
 * - Los nodos que pertenecen a la solución reciben valores aleatorios en el rango [0.55, 0.95].
 * - Los nodos que **NO** pertenecen a la solución reciben valores aleatorios en el rango [0.05, 0.45].
 *
 * Este tipo de codificación permite que el cromosoma preserve información aproximada
 * sobre qué nodos forman parte de la solución, facilitando la búsqueda local o el crossover
 * en el espacio de soluciones continuo del BRKGA.
 *
 * @param size_graph Tamaño del grafo (número total de nodos).
 * @param solution Vector con los índices de los nodos que forman parte de la solución.
 *
 * @return Un objeto `BRKGA::Chromosome` de longitud `size_graph`, donde cada gen
 *         contiene un valor aleatorio en el rango apropiado dependiendo de si el nodo
 *         pertenece o no a la solución.
 *
 * @throws std::out_of_range Si algún índice de `solution` es negativo o excede el tamaño del grafo.
 */
BRKGA::Chromosome encode(int size_graph, const std::vector<int>& solution) {
    BRKGA::Chromosome chromosome(size_graph);
    std::vector<bool> in_solution(size_graph, false);

    // Marcar los nodos que están en la solución
    for (int node : solution) {
        if (node < 0 || node >= size_graph)
            throw std::out_of_range("Nodo de la solución fuera de rango");

        in_solution[node] = true;
    }

    // Generador de números aleatorios
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist_in(0.55, 0.95);
    std::uniform_real_distribution<double> dist_out(0.05, 0.45);
    
    // Asignar valores de random keys
    for (int i = 0; i < size_graph; i++)
        chromosome[i] = in_solution[i] ? dist_in(gen) : dist_out(gen);

    return chromosome;
}
