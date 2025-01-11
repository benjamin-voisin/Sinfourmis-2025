#pragma once

#include "graph.hpp"
#include <cstdint>
#include <vector>

// Un chemin dans le graph d'un point à un autre, avec le coût associé à la fin.
// Le coût est rest par le passage dans un noeud avec de l'eau, donc on a le coût
// en eau depuis le dernier point d'eau traversé.
class Path {
  private:
    node_id _from;
    node_id _to;
    std::vector<node_id> _edges_to_take;
    uint32_t _water_cost;
    uint32_t _time_cost;

  public:
    Path(node_id start): _from(start), _to(start), _edges_to_take({}), _water_cost(0), _time_cost(0) {};

    // Extends the graph by one node, updating its total cost
    void extend(Node node, Edge edge);

    // Checks if the path is empty
    bool is_empty() {
      return _edges_to_take.empty();
    }

    // Returns this path's start node.
    node_id start() {
      return _from;
    }

    // Returns this path's current end node.
    node_id end() {
      return _to;
    }

    // Returns the water cost of that path.
    uint32_t water_cost() {
      return _water_cost;
    }

    // Returns the time it takes to go along that path.
    uint32_t time_cost() {
      return _time_cost;
    }

    // Returns the current path to take.
    const std::vector<node_id>& path() {
      return _edges_to_take;
    }
};
