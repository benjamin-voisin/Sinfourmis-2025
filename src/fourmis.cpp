#include "sinfourmis.h"
#include "thread_queue.h"

#include <iostream>
#include <optional>
#include <thread>
#include <vector>

std::thread* queen_thread = nullptr;

typedef struct {
    std::vector<fourmi_etat> forumis_miam_miam;
    const reine_etat* state;
    const salle* node;
} reine_input_t;

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

        std::cout << "Yay, on m'a donné des infos, et c'est déjà la " << called << " ième fois" << std::endl;
        called += 1;

        // Et on renvoit notre retour qu'on veut, voilà
        from_reine.send_message({ .action = REINE_PASSE, .arg = called });
    }
}

reine_retour cpp_reine_activation(fourmi_etat fourmis[], const unsigned int nb_fourmis,
                                  const reine_etat *etat, const salle *salle) {
    // Lance le thread si c'est pas déjà le cas oh non.
    if (queen_thread == nullptr) {
        std::cout << "G lancé la reine" << std::endl;
        queen_thread = new std::thread(reine_thread);
    }

    std::vector<fourmi_etat> ouvrieres(fourmis, fourmis + nb_fourmis);

    // Send info to the queen thread
    to_reine.send_message({.forumis_miam_miam = ouvrieres, .state = etat, .node = salle });

    // Wait for return from the queen thread
    std::optional<reine_retour> queen_return = std::nullopt;
    while (!queen_return.has_value()) {
        queen_return = from_reine.try_receive_message();
    }
    std::cout << "La reine m'a renvoyé un arg: " << queen_return.value().arg << std::endl;
    
    return queen_return.value();
}

fourmi_retour cpp_fourmi_activation(fourmi_etat *etat, const salle *salle) {
    std::cout << "prout" << std::endl;
    return {.action = FOURMI_PASSE, .arg = 0, .depose_pheromone = NO_PHEROMONE, .pheromone = 0};
}

extern "C" {
reine_retour reine_activation(fourmi_etat fourmis[], const unsigned int nb_fourmis,
                              const reine_etat *etat, const salle *salle) {
    return cpp_reine_activation(fourmis, nb_fourmis, etat, salle);
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
    return cpp_fourmi_activation(etat, salle);
}
}
