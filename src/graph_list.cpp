#include "graph_list.h"
#include <algorithm>

GraphList::GraphList(int num_vertex){
    this->num_vertex = num_vertex;
    num_edges = 0;
    adjacency_list = std::vector(num_vertex, std::vector<int>());
}

bool GraphList::has_edge(int vertex_1, int vertex_2) const {
    if (adjacency_list[vertex_1].size() < adjacency_list[vertex_2].size()){
        return std::any_of(
            adjacency_list[vertex_1].begin(),
            adjacency_list[vertex_1].end(),
            [vertex_2](int x) { return x == vertex_2; }
        );
    }
    return std::any_of(
        adjacency_list[vertex_2].begin(),
        adjacency_list[vertex_2].end(),
        [vertex_1](int x) { return x == vertex_1; }
    );
}

void GraphList::add_edge(int vertex_1, int vertex_2){
    if (this->has_edge(vertex_1, vertex_2))
        return;

    adjacency_list[vertex_1].push_back(vertex_2);
    adjacency_list[vertex_2].push_back(vertex_1);

    num_edges++;
}

void GraphList::remove_edge(int vertex_1, int vertex_2){
    if (!this->has_edge(vertex_1, vertex_2))
        return;

    for (auto it = adjacency_list[vertex_1].begin(); it != adjacency_list[vertex_1].end(); it++){
        if (*it == vertex_2){
            adjacency_list[vertex_1].erase(it);
            break;
        }
    }
    for (auto it = adjacency_list[vertex_2].begin(); it != adjacency_list[vertex_2].end(); it++){
        if (*it == vertex_1){
            adjacency_list[vertex_2].erase(it);
            break;
        }
    }
    num_edges--;
}

int GraphList::degree(int vertex) const {
    return adjacency_list[vertex].size();
}

int GraphList::get_num_vertices() const {
    return this->num_vertex;
}

int GraphList::get_num_edges() const {
    return this->num_edges;
}

std::vector<int> GraphList::get_neighbors(int vertex) const {
    return adjacency_list[vertex];
}
