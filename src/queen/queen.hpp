#pragma once 

#include "../graph.hpp"
#include "../sinfourmis.h"
#include "thread_queue.h"
#include <thread>

class Queen {
	private:
		Graph _graph;
	public:
		Queen(): _graph() {};
		Graph* get_graph();
};

extern std::thread* queen_thread;

typedef struct {
    std::vector<fourmi_etat> forumis_miam_miam;
    const reine_etat* state;
    const salle* node;
} reine_input_t;

extern ThreadQueue<reine_input_t> to_reine;
extern ThreadQueue<reine_retour> from_reine;

void reine_thread();

