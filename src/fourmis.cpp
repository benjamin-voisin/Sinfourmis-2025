#include "sinfourmis.h"
#include "queen/queen.hpp"

#include <iostream>
#include <optional>
#include <vector>


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
