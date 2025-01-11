#include "graph.hpp"

#include <iostream>

Node::Node(node_type_t type, node_data_t data, node_id id) : _type{type}, _data{data}, _id{id}	{}

node_id Node::get_id() {
	return _id;
}

Edge::Edge(node_id target, size_t cost, uint8_t degree_entrant, uint8_t degree_sortant) : _cost{cost}, _target_id{target}, _life{1} {}

node_id Edge::get_target() {
	return _target_id;
}

Graph::Graph() : _n_nodes{0}  {}

const bool Graph::is_empty() {
	return _n_nodes == 0;
}

void Graph::add_node(node_type_t type, node_data_t data, node_id id) {
	if (!_nodes[id].has_value()) {
		_nodes[id] = Node(type, data, id);
		_n_nodes++;
	}
}

void Graph::add_edge(node_id node1, node_id node2, size_t cost, uint8_t degree_entrant, uint8_t degree_sortant) {
	// D’abord on regarde si cette arrête existe pas déjà
	bool exists_e1_2 = false;
	for (Edge edge : _adjacency[node1]) {
		if (edge.get_target() == node2) exists_e1_2 = true;
	}
	bool exists_e2_1 = false;
	for (Edge edge : _adjacency[node2]) {
		if (edge.get_target() == node2) exists_e2_1 = true;
	}
	if (exists_e1_2 != exists_e2_1) {
		std::cerr << "Error in the graph" << std::endl;
		exit(1);
	};
	// Si l’arrête existe pas déjà
	if (!exists_e1_2 && !exists_e2_1) {
		// On ajoute l’arrête dans les listes d’adjacences des deux nœuds
		_adjacency[node1].push_back(Edge(node2, cost, degree_entrant, degree_sortant));
		_adjacency[node2].push_back(Edge(node1, cost, degree_entrant, degree_sortant));
	}

}

void Graph::remove_edge(node_id node1, node_id node2) {
	// On cherche l’addresse de l’edge n1 -> n2
	size_t index = -1;
	for (size_t i = 0; i < _adjacency[node1].size(); i++) {
		if (_adjacency[node1][i].get_target() == node2) { // On a trouvé l’arrête
			index = i;
			break;
		}
	}
	// On swap la dernière arrête avec celle à enlever
	_adjacency[node1][index] = _adjacency[node1][_adjacency[node1].size()];
	// Puis on pop
	_adjacency[node1].pop_back();

	// Et on refait la même chose dans l’autre sens
	// On cherche l’addresse de l’edge n2 -> n1
	index = -1;
	for (size_t i = 0; i < _adjacency[node2].size(); i++) {
		if (_adjacency[node2][i].get_target() == node1) { // On a trouvé l’arrête
			index = i;
			break;
		}
	}
	// On swap la dernière arrête avec celle à enlever
	_adjacency[node2][index] = _adjacency[node2][_adjacency[node2].size()];
	// Puis on pop
	_adjacency[node2].pop_back();

}
