#pragma once
#include <string>
#include "graph_matrix.h"
#include "graph_list.h"

namespace graph_creation {
    GraphMatrix read_graph(const std::string& ruta);
    GraphList read_graph_adjacent(const std::string& ruta);
}