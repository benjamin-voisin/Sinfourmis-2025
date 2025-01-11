#pragma once

#include <cstdint>
#include <vector>
#include <optional>

class Path;

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
	public:
		node_type_t type; // Le type du nœud, water, food, queen, empty
		node_data_t data; // La data dans le nœud
		node_id id;

		Node(node_type_t type, node_data_t data, node_id id);
		node_id get_id();
};

class Edge {
	public:
		size_t cost;
		uint8_t life;
		node_id target_id;
		uint8_t degree_entrant; // Le numéro d’arrête dans le nœud qu’il faut prendre pour prendre cette arrête
		uint8_t degree_sortant; // Le numéro d’arrête qu’il faut prendre dans le nœud d’arrivée pour retourner au nœeud d’avant
		Edge(node_id target, size_t cost, uint8_t degree_entrant, uint8_t degree_sortant);
		node_id get_target();
};

class Graph {
	private:
		std::vector<Edge> _adjacency[256];
		std::optional<Node> _nodes[256];
		int _n_nodes;
	public:
		const bool is_empty();
		void add_node(node_type_t tag, node_data_t data, node_id id);
		void add_edge(node_id node1, node_id node2, size_t cost, uint8_t degree_entrant, uint8_t degree_sortant);
		void remove_edge(node_id node1, node_id node2);

		std::vector<Path> compute_shortest_paths(node_id source);

		Graph();
};
