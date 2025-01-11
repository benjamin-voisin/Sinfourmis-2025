#include "graph.hpp"

#include <iostream>

Node::Node(node_type_t type, node_data_t data, node_id id) : type{type}, data{data}, id{id}	{}

Edge::Edge(node_id target, size_t cost) : cost{cost}, target_id{target}, life{1} {}

Graph::Graph() : _n_nodes{0}  {}

void Graph::add_node(node_type_t type, node_data_t data, node_id id) {
	_nodes[id] = Node(type, data, id);
}

void Graph::add_edge(node_id node1, node_id node2, size_t cost) {
	// D’abord on regarde si cette arrête existe pas déjà
	bool exists_e1_2 = false;
	for (Edge edge : _adjacency[node1]) {
		if (edge.target_id == node2) exists_e1_2 = true;
	}
	bool exists_e2_1 = false;
	for (Edge edge : _adjacency[node2]) {
		if (edge.target_id == node2) exists_e2_1 = true;
	}
	if (exists_e1_2 != exists_e2_1) {
		std::cerr << "Error in the graph" << std::endl;
		exit(1);
	};
	// Si l’arrête existe pas déjà
	if (!exists_e1_2 && !exists_e2_1) {
		// On ajoute l’arrête dans les listes d’adjacences des deux nœuds
		_adjacency[node1].push_back(Edge(node2, cost));
		_adjacency[node2].push_back(Edge(node1, cost));
	}

}

void Graph::remove_edge(node_id node1, node_id node2) {
	// On cherche l’addresse de l’edge n1 -> n2
	size_t index = -1;
	for (size_t i = 0; i < _adjacency[node1].size(); i++) {
		if (_adjacency[node1][i].target_id == node2) { // On a trouvé l’arrête
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
		if (_adjacency[node2][i].target_id == node1) { // On a trouvé l’arrête
			index = i;
			break;
		}
	}
	// On swap la dernière arrête avec celle à enlever
	_adjacency[node2][index] = _adjacency[node2][_adjacency[node2].size()];
	// Puis on pop
	_adjacency[node2].pop_back();

}
