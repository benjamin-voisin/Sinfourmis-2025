#include "main.h"

#include "../sinfourmis.h"

#include "reine.h"

#include "api/basics.h"

reine_retour reine_activation(fourmi_etat fourmis[],
                              const unsigned int nb_fourmis,
                              const reine_etat *etat, const salle *salle) {
    static reine_t* reine = reine_alloc();
    Assert(CAT_REINE, reine != NULL, "La struct REINE est NULL.");
    reine->tick++;
    reine_pp(CAT_REINE, LOG_WARNING, reine);
    if (reine->tick == 1000) {
        Log(CAT_REINE, LOG_INFO, "Free Reine struct.");
        reine_free(reine);
    }
    return reine_action_passe();
}