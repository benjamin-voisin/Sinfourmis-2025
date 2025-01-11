#pragma once 

#include "../graph.hpp"
#include "../sinfourmis.h"
#include "thread_queue.h"
#include <thread>

class Queen {
	private:
		Graph _graph;
		reine_etat _last_state;
		uint32_t _ticks;
	public:
		Queen(): _graph(), _last_state(), _ticks(0) {};
		Graph* graph();

		reine_etat last_state();
		void register_last_state(reine_etat state);
		uint32_t ticks();
		void update_tick_counter(reine_action action);

		void read_scouts(const std::vector<fourmi_etat>& states);
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

