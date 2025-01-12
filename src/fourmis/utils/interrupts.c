#include "interrupts.h"
#include "pile.h"
#include "log.h"
#include "../../constants.hpp"
#include "../commun.h"

enum fourminterrupt_e interrupt(fourmi_etat *etat, const salle *salle) {
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
    if ((etat->eau < mem->stats.max_eau / 2 + 7) && !(pile_vide(etat->memoire))) {
        Log_info(CAT_FOURMIS, "INTERRUPT_WATER DETECTEE\n");
        return INTERRUPT_WATER;
    }
    
    if (salle->taille_liste > 1) {
        Log_info(CAT_FOURMIS, "INTERRUPT_ENNEMY DETECTEE\n");
        return INTERRUPT_ENNEMY;
    }
    if ((etat->vie == 1) && !(pile_vide(etat->memoire))) {
        Log_info(CAT_FOURMIS, "INTERRUPT_LIFE DETECTEE\n");
        return INTERRUPT_LIFE;
    }
    return AUCUN_INTERRUPT;
}