#pragma once
#include <cstdint>
#include <vector>

class GraphMatrix {
private:
    int num_vertex;
    int num_edges;
    std::vector<std::vector<uint8_t>> adjacency_matrix;
    std::vector<int> degrees;

public:
    // Constructor
    explicit GraphMatrix(int num_vertex);

    // Métodos principales
    bool has_edge(int vertex_1, int vertex_2) const;
    void add_edge(int vertex_1, int vertex_2);
    void remove_edge(int vertex_1, int vertex_2);

    // Información del grafo
    int degree(int vertex) const;
    int get_num_vertices() const;
    int get_num_edges() const;

    // Vecinos de un vértice
    std::vector<int> get_neighbors(int vertex) const;
};
