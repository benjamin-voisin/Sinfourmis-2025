#include "queen.hpp"
#include "../sinfourmis.h"
#include "../graph.hpp"
#include "read_scout.hpp"
#include "thread_queue.h"
#include "../fourmis/scout.h"
#include <algorithm>
#include <cstdint>
#include <thread>
#include <vector>

std::thread* queen_thread = nullptr;
ThreadQueue<reine_input_t> to_reine;
ThreadQueue<reine_retour> from_reine;

reine_retour give_args_to_thread(
    fourmi_etat fourmis[],
    const unsigned int nb_fourmis,
    const reine_etat *etat, const salle *salle
) {
  // Lance le thread si c'est pas déjà le cas oh non.
  if (queen_thread == nullptr) {
    queen_thread = new std::thread(reine_thread);
  }

  std::vector<fourmi_etat> ouvrieres_garage(fourmis, fourmis + nb_fourmis);

  

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

        // Ajoute le noeud originel si jamais y'en a po
        if (queen_state.graph()->is_empty()) {
            node_data_t data;
            data.queen = {.friendly = true, .tag = 0};
            queen_state.graph()->add_node(YAS_QUEEN, data, 0);
        }

        // Update les infos par les scouts
        queen_state.read_scouts(input.forumis_miam_miam);

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
        // On rappel les fourmis si on a la place et totu
        auto storage_space = input.state->max_stockage - input.forumis_miam_miam.size();
        if (storage_space <= friendly_ants_present) {
            action = RECUPERER_FOURMI;
            arg = friendly_ants_present;
        } else if (!input.forumis_miam_miam.empty()) {
            // Sinon, on vide un peu le stockage
			// On initialise la mémoire de totute les fourmis
			for (fourmi_etat fourmis : input.forumis_miam_miam) {
				scout_loads(&fourmis, input.state->team_id, {0}, 0, 1);
			}
            action = ENVOYER_FOURMI;
            arg = std::min((uint32_t)input.forumis_miam_miam.size(), input.state->max_envoi);
        } else {
            // Sinon on fabrique totu les forumis ahah forumi go brrrrrrrrrrrrrrrrrrrrr
            action = CREER_FOURMI;
            arg = input.state->max_production;
        }

        // Et on renvoit notre retour qu'on veut, voilà
        from_reine.send_message({ .action = action, .arg = arg });
        queen_state.update_tick_counter(action);
    }
}


// Met à jour le graph de la reine selon l'état des fourmis présentes.
void Queen::read_scouts(const std::vector<fourmi_etat>& states) {
    for (auto state : states) {
        read_scout(&this->_graph, state);
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
        case AMELIORE_DEGATS: ticks = 10;
        default: ticks = 1;
    }
    _ticks += ticks;
}
