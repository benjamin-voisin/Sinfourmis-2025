#pragma once

#include "graph.hpp"
#include <cstdint>
#include <vector>

// Un chemin dans le graph d'un point à un autre, avec le coût associé à la fin.
// Le coût est rest par le passage dans un noeud avec de l'eau, donc on a le coût
// en eau depuis le dernier point d'eau traversé.
class Path {
  public:
    node_id from;
    node_id to;
    std::vector<node_id> edges_to_take;
    uint32_t water_cost;
    uint32_t time_cost;

    Path(): from(0), to(0), edges_to_take({}), water_cost(0), time_cost(0) {};
    void extend(Node node, uint32_t edge_cost);
};
