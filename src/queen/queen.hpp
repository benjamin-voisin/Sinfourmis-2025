#pragma once 

#include "graph.hpp"
#include "../sinfourmis.h"
#include "thread_queue.h"
#include "../fourmis/utils/pile.h"
#include "reine_input.hpp"
#include "scheduler.hpp"

#include <thread>
#include <optional>

class Queen {
	private:
		Graph _graph;
		reine_etat _last_state;
		uint32_t _ticks;
		uint32_t _produced_ants;

	public:
		Scheduler _scheduler;

		std::optional<std::vector<pile_t>> path_to_node[256];
		node_id _next_manger_target;
		uint8_t next_scout = 1;

		Queen();
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

extern ThreadQueue<reine_input_t> to_reine;
extern ThreadQueue<reine_retour> from_reine;

reine_retour give_args_to_thread(
  fourmi_etat fourmis[],
  const unsigned int nb_fourmis,
  const reine_etat *etat, const salle *salle
);
void reine_thread();

