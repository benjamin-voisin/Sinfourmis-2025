
#include "../sinfourmis.h"
#include "queen.hpp"

reine_retour reine_activation(fourmi_etat fourmis[],
                              const unsigned int nb_fourmis,
                              const reine_etat *etat, const salle *salle) {
  return give_args_to_thread(fourmis, nb_fourmis, etat, salle);
}
