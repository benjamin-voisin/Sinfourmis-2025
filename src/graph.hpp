#pragma once

#include <cstdint>
#include <vector>
#include <optional>

typedef enum node_type_e {
	WATER,
	FOOD,
	YAS_QUEEN,
	EMPTY,
} node_type_t;

typedef union node_data_u {
	struct {
		size_t regen_speed;
		size_t max_storage;
		size_t max_produced;
		size_t taken;
	} food;
	struct {
		bool friendly;
		uint8_t tag;
	} queen;
} node_data_t;

typedef uint8_t node_id;

class Node {
	private:
		node_type_t _type; // Le type du nœud, water, food, queen, empty
		node_data_t _data; // La data dans le nœud
		node_id _id;
	public:
		Node(node_type_t type, node_data_t data, node_id id);
		node_id get_id();
};

class Edge {
	private:
		size_t _cost;
		uint8_t _life;
		node_id _target_id;
	public:
		Edge(node_id target, size_t cost);
		node_id get_target();
};

class Graph {
	private:
		std::vector<Edge> _adjacency[256];
		std::optional<Node> _nodes[256];
		int _n_nodes;
	public:
		void add_node(node_type_t tag, node_data_t data, node_id id);
		void add_edge(node_id node1, node_id node2, size_t cost);
		void remove_edge(node_id node1, node_id node2);
		Graph();
};
