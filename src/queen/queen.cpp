#include "queen.hpp"
#include "../sinfourmis.h"
#include "graph.hpp"
#include "read_scout.hpp"
#include "scheduler.hpp"
#include "thread_queue.h"
#include "../fourmis/main.h"
#include "../fourmis/utils/b_constants.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

std::thread* queen_thread = nullptr;
ThreadQueue<reine_input_t> to_reine;
ThreadQueue<reine_retour> from_reine;

Queen::Queen(): _graph(), _last_state(), _ticks(0), _produced_ants(0) {
	for (int i = 0; i < 256; i++) {
		path_to_node[i] = std::nullopt;
	}
	arg_t arg = {0};
	arg.amount = 1;
	_scheduler.add_task(Task(CREER_SCOUT, arg ));
	_scheduler.add_task(Task(GASLIGHT_SCOUT, arg));
	_scheduler.add_task(Task(PASS, arg));
};

reine_retour give_args_to_thread(
    fourmi_etat* fourmis,
    const unsigned int nb_fourmis,
    const reine_etat *etat, const salle *salle
) {
  // Lance le thread si c'est pas déjà le cas oh non.
  if (queen_thread == nullptr) {
    queen_thread = new std::thread(reine_thread);
  }

  std::vector<fourmi_etat*> ouvrieres_garage(&fourmis, &fourmis + nb_fourmis);

  // Send info to the queen thread
  to_reine.send_message(
      {.forumis_miam_miam = ouvrieres_garage, .state = etat, .node = salle});

  // Wait for return from the queen thread
  return from_reine.wait_message();
}

void reine_thread() {
    Queen queen_state;
    // La reine elle travaille totu le temps pour slay les ennemis
    while (true) {
        // On attends l'input de la lib
        auto input = to_reine.wait_message();
        queen_state.register_last_state(*input.state);

        std::cerr << "[QUEEN] Woke up on tick " << queen_state.ticks() << std::endl;

        if (queen_state.ticks() % 20 == 0) {
            std::cerr << "[QUEEN] Wrote graph to ./graph.dot" << std::endl;
            queen_state.graph()->to_dot("graph.dot");
        }

        // Ajoute le noeud originel si jamais y'en a po
        if (queen_state.graph()->is_empty()) {
            std::cerr << "[QUEEN] Init graph" << std::endl;
            node_data_t data;
            data.queen = {.friendly = true, .tag = 0};
            queen_state.graph()->add_node(YAS_QUEEN, data, 0);
        }

        // Update les infos par les scouts
        /* queen_state.read_scouts(input.forumis_miam_miam); */
		/* queen_state.graph()->to_dot("graph.dot"); */
        std::vector<fourmis_compteur> ants_present(input.node->compteurs_fourmis, input.node->compteurs_fourmis + input.node->taille_liste);
        uint32_t friendly_ants_present = 0;
        for (auto compteur : ants_present) {
            if (compteur.equipe == input.state->team_id) {
                friendly_ants_present = compteur.nombre;
                break;
            }
        }
        std::cerr << "[QUEEN] Friendly ants in node: " << friendly_ants_present << ", ants in garage: " << input.forumis_miam_miam.size() << std::endl;
        // On rappel les fourmis si on a la place et totu
        auto storage_space = input.state->max_stockage - input.forumis_miam_miam.size();

		// On update nos chemins
		for (fourmi_etat *fourmis : input.forumis_miam_miam) {
			size_t pile_size;
			pile_t *pile = pile_dumps(fourmis->memoire, &pile_size);
			if (pile != NULL && fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
				// Si le chemin est plus court que ce qu’on conanit
				if (!queen_state.path_to_node[pile->id].has_value() || queen_state.path_to_node[pile->id].value().size() > pile_size) {
					std::cout << "Trouvé un chemi plu court\n";
					std::vector<pile_t> v;
					for (size_t i = 0; i < pile_size; i++) {
						v.push_back(*(pile+i));
					}
					queen_state.path_to_node[pile->id] = v;
					if (pile->type == NOURRITURE) {
						// On va spawn une fourmi manger
						std::cout << "MANGER\n";
						arg_t arg = {0};
						arg.manger_target = pile->id;
						arg.amount = 1;
						queen_state._scheduler.add_task(Task(GASLIGHT_SCOUT, arg));

						size_t number_of_ants = 10;

						for (size_t i = 0; i < number_of_ants; i++) {
							queen_state._scheduler.add_task(Task(PASS, arg));
							queen_state._scheduler.add_task(Task(CREER_MANGER, arg));
							queen_state._scheduler.add_task(Task(GASLIGHT_MANGER, arg));
						}

					}
				}
			}
		}

        // Cherche la prochaine action à faire
        reine_action action;
        int arg = 0;

		queen_state._scheduler.execute_tasks(&action, &arg, &input, friendly_ants_present, queen_state.path_to_node, queen_state.stat);
		
        // Et on renvoit notre retour qu'on veut, voilà
        from_reine.send_message({ .action = action, .arg = arg });
        queen_state.update_tick_counter(action);
    }
}


// Met à jour le graph de la reine selon l'état des fourmis présentes.
void Queen::read_scouts(const std::vector<fourmi_etat*>& states) {
    for (auto state : states) {
        read_scout(&this->_graph, *state);
    }
}

Graph* Queen::graph() { return &_graph; }

reine_etat Queen::last_state() {
    return _last_state;
}
void Queen::register_last_state(reine_etat state) {
    _last_state = state;
}
uint32_t Queen::ticks() {
    return _ticks;
}
void Queen::update_tick_counter(reine_action action) {
    auto ticks = 1;
    switch (action) {
        case AMELIORE_STOCKAGE:
        case AMELIORE_PRODUCTION:
        case AMELIORE_ENVOI:
        case AMELIORE_VITESSE_AMELIORATION:
        case AMELIORE_RAMASSAGE:
        case AMELIORE_VIE:
        case AMELIORE_EAU:
        case AMELIORE_DEGATS: 
            ticks = 10; 
            break;
        default: ticks = 1;
    }
    _ticks += ticks;
}
uint32_t Queen::produced_ants() {
    return this->_produced_ants;
}
void Queen::produce_ants(uint32_t amount) {
    this->_produced_ants += amount;
}
