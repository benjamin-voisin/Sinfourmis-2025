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
		uint32_t _produced_ants;
	public:
		Queen(): _graph(), _last_state(), _ticks(0), _produced_ants(0) {};
		Graph* graph();

		reine_etat last_state();
		void register_last_state(reine_etat state);
		uint32_t ticks();
		void update_tick_counter(reine_action action);
		uint32_t produced_ants();
		void produce_ants(uint32_t amount);

		void read_scouts(const std::vector<fourmi_etat*>& states);
};

extern std::thread* queen_thread;

typedef struct {
    std::vector<fourmi_etat*> forumis_miam_miam;
    const reine_etat* state;
    const salle* node;
} reine_input_t;

extern ThreadQueue<reine_input_t> to_reine;
extern ThreadQueue<reine_retour> from_reine;

reine_retour give_args_to_thread(
  fourmi_etat fourmis[],
  const unsigned int nb_fourmis,
  const reine_etat *etat, const salle *salle
);
void reine_thread();

