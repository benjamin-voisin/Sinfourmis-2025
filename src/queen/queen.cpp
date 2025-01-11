#include "queen.hpp"
#include "../sinfourmis.h"
#include "thread_queue.h"
#include <thread>

std::thread* queen_thread = nullptr;
ThreadQueue<reine_input_t> to_reine;
ThreadQueue<reine_retour> from_reine;

void reine_thread() {
    int called = 0;
    // La reine elle travaille totu le temps pour slay les ennemis
    while (true) {
        // On attends l'input de la lib
        auto input = to_reine.try_receive_message();
        if (!input.has_value()) {
            continue;
        }

        called += 1;

        // Et on renvoit notre retour qu'on veut, voilà
        from_reine.send_message({ .action = REINE_PASSE, .arg = called });
    }
}
