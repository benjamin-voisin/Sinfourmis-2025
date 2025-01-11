#include "graph.hpp"
#include "path.hpp"

void Path::extend(Node node, Edge edge) {
  if (node.type == WATER) {
    // Reset water cost
    _water_cost = 0;
  }

  _time_cost += edge.cost;
  _edges_to_take.push_back(edge.degree_entrant);
}
