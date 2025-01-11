#include "queen.hpp"
#include "../sinfourmis.h"
#include "../graph.hpp"
#include "read_scout.hpp"
#include "thread_queue.h"
#include <thread>
#include <vector>

std::thread* queen_thread = nullptr;
ThreadQueue<reine_input_t> to_reine;
ThreadQueue<reine_retour> from_reine;

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

        queen_state.read_scouts(input.forumis_miam_miam);

        // Et on renvoit notre retour qu'on veut, voilà
        auto action = REINE_PASSE;
        from_reine.send_message({ .action = action, .arg = 0 });
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
