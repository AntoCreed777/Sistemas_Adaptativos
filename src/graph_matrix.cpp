#include "graph_matrix.h"

GraphMatrix::GraphMatrix(int num_vertex){
    this->num_vertex = num_vertex;
    num_edges = 0;
    adjacency_matrix = std::vector(num_vertex, std::vector<uint8_t>(num_vertex, false));
    degrees = std::vector<int>(num_vertex, 0);
}

bool GraphMatrix::has_edge(int vertex_1, int vertex_2) const {
    return adjacency_matrix[vertex_1][vertex_2];
}

void GraphMatrix::add_edge(int vertex_1, int vertex_2){
    if (adjacency_matrix[vertex_1][vertex_2])
        return;

    adjacency_matrix[vertex_1][vertex_2] = 1;
    adjacency_matrix[vertex_2][vertex_1] = 1;

    degrees[vertex_1] += 1;
    degrees[vertex_2] += 1;
    num_edges++;
}

void GraphMatrix::remove_edge(int vertex_1, int vertex_2){
    if (!adjacency_matrix[vertex_1][vertex_2])
        return;
    adjacency_matrix[vertex_1][vertex_2] = 0;
    adjacency_matrix[vertex_2][vertex_1] = 0;
    degrees[vertex_1] -= 1;
    degrees[vertex_2] -= 1;
    num_edges--;
}

void GraphMatrix::remove_vertex(int vertex) {
    for (auto neighbors_vertex: adjacency_matrix[vertex]) {
        remove_edge(vertex, neighbors_vertex);
    }
    degrees[vertex] = -1;   // Eliminado logico
}

void GraphMatrix::remove_neighbors_of_vertex(int vertex) {
    auto neighbors = get_neighbors(vertex);
    for (int neighbor : neighbors) {
        remove_vertex(neighbor);
    }
}

void GraphMatrix::remove_neighbors_of_vertex_and_this_vertex(int vertex) {
    remove_neighbors_of_vertex(vertex);
    remove_vertex(vertex);
}


int GraphMatrix::get_degree(int vertex) const {
    return degrees[vertex];
}

int GraphMatrix::get_num_vertices() const {
    return this->num_vertex;
}

int GraphMatrix::get_num_edges() const {
    return this->num_edges;
}

bool GraphMatrix::is_empty() const {
    for (auto i = 0; i < this->num_vertex; i++)
        if (this->get_degree(i) >= 0)
            return false;

    return true;
}

std::vector<int> GraphMatrix::get_neighbors(int vertex) const {
    std::vector<int> neighbors;
    const std::vector<uint8_t>& row = adjacency_matrix[vertex];
    neighbors.reserve(degrees[vertex]); 

    for (size_t i = 0; i < row.size(); i++){
        if (row[i])
            neighbors.push_back(i);
    }
    return neighbors;
}
