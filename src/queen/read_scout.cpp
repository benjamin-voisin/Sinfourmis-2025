#include "read_scout.hpp"
#include "../fourmis/utils/pile.h"

#include <cstdint>
#include <iostream>
#include <assert.h>

node_type_t type_to_node_type(uint8_t type) {
	switch (type) {
		case VIDE:
			return EMPTY;
		case EAU:
			return WATER;
		case NOURRITURE:
			return FOOD;
		case REINE:
			return YAS_QUEEN;
		default:
			std::cerr << "Type non reconnu" << std::endl;
			assert(false);
	}
}

void read_scout(Graph *graph, fourmi_etat fourmi) {
	size_t taille;
	pile_t *stack_pointer = pile_dumps(fourmi.memoire, &taille);
	for (size_t i = 0; i < taille; i++) {
		// Check si on est dans le dernier noeud
		node_id source;
		if (i == taille - 1) {
			source = 0;
		} else {
			source = (stack_pointer+1)->id;
			graph->add_node(type_to_node_type((stack_pointer+1)->type), {0}, source);
		}
		graph->add_node(type_to_node_type(stack_pointer->type), {0}, stack_pointer->id);
		graph->add_edge(source, stack_pointer->id, stack_pointer->poid, stack_pointer->degree_entrant, stack_pointer->degree_sortant);
		stack_pointer++;
	}
}
