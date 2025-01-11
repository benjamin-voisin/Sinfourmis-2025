#pragma once

#include "graph.hpp"
#include <vector>

// Un chemin dans le graph d'un point à un autre, avec le coût associé à la fin.
// Le coût est rest par le passage dans un noeud avec de l'eau, donc on a le coût
// en eau depuis le dernier point d'eau traversé.
class Path {
  public:
    node_id from;
    node_id to;
    std::vector<node_id> edges_to_take;

    Path(): from(0), to(0), edges_to_take({}) {};
  
};
