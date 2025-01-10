#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

typedef enum node_tag_e {
  WATER,
  FOOD,
  YAS_QUEEN,
} node_tag_t;

typedef union node_data_u {
  struct {
    uint8_t regen_speed;
    uint8_t max_storage;
    uint8_t max_produced;
    uint8_t taken;
  } food;
  struct {
    bool friendly;
    uint8_t tag;
  } queen;
} node_data_t;

typedef struct node_s {
  node_tag_t tag;
  node_data_t data;
  uint8_t life;
} node_t;

typedef struct edge_s {
  size_t target;
  uint8_t life;
} edge_t;

typedef struct graph_s {
  private:
    std::vector<std::vector<edge_t>> adjacency;
    std::vector<node_t> nodes;
} graph_t;

void add_node(node_tag_t tag, node_data_t data) {
  
}
void add_edge(size_t node1, size_t node2) {
  
}
void remove_edge(size_t node1, size_t node2) {
  
}

