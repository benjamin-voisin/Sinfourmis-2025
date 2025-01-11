#include "queen.hpp"
#include "../sinfourmis.h"
#include "graph.hpp"
#include "read_scout.hpp"
#include "thread_queue.h"
#include "../fourmis/main.h"
#include "../fourmis/scout.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

std::thread* queen_thread = nullptr;
ThreadQueue<reine_input_t> to_reine;
ThreadQueue<reine_retour> from_reine;

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

        // Cherche la prochaine action à faire
        auto action = REINE_PASSE;
        auto arg = 0;

        // Vérifie si il y a des fourmis sur la case
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
        if (storage_space >= friendly_ants_present && friendly_ants_present > 0) {
            action = RECUPERER_FOURMI;
            arg = friendly_ants_present;
            std::cerr << "[QUEEN] Calling back " << arg << " ants in garage" << std::endl;
        } else if (!input.forumis_miam_miam.empty()) {
            // Sinon, on vide un peu le stockage
        		// On initialise la mémoire de totute les fourmis
        		for (fourmi_etat* fourmis : input.forumis_miam_miam) {
        			fourmi_pp(stdout, fourmis);
    					std::cerr << "[QUEEN] gaslight ant\n";
        			scout_loads(fourmis, input.state->team_id, NULL, 0, queen_state.produced_ants() << 3);
        		}
            action = ENVOYER_FOURMI;
            arg = std::min((uint32_t)input.forumis_miam_miam.size(), input.state->max_envoi);
            std::cerr << "[QUEEN] Sent " << arg << " ants from garage" << std::endl;
        } else if (queen_state.produced_ants() < 5 && input.state->nourriture > 15) {
            // On créé des potites froumis si on a de la bouffe et qu'on en a moins de 5
            /* action = CREER_FOURMI; */
            arg = std::min((input.state->max_nourriture - 10) / 10, input.state->max_production);
            queen_state.produce_ants(arg);
            std::cerr << "[QUEEN] Created " << arg << " ants" << std::endl;
        }

		if (queen_state.ticks() == 1) {
			action = CREER_FOURMI;
			arg = 1;
            queen_state.produce_ants(arg);
		}
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
