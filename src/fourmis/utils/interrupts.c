#include "interrupts.h"
#include "pile.h"
#include "log.h"
#include "../../constants.hpp"
#include "../commun.h"

void interrupt_incr(fourmi_etat *etat, fourminterrupt_t inter) {
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
    assert(inter < NB_INTERRUPTS);
    
    if (mem->interrupt_stats[inter] < UINT8_MAX)
        mem->interrupt_stats[inter]++;
}

fourminterrupt_t interrupt(fourmi_etat *etat, const salle *salle) {
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
    if ((etat->eau < mem->stats.max_eau / 2 + 7) && !(pile_vide(etat->memoire))) {
        Log_warning(CAT_FOURMIS, "INTERRUPT_WATER DETECTEE\n");
        interrupt_incr(etat, INTERRUPT_WATER);
        return INTERRUPT_WATER;
    }
    
    if (salle->taille_liste > 1) {
        Log_info(CAT_FOURMIS, "INTERRUPT_ENNEMY DETECTEE\n");
        interrupt_incr(etat, INTERRUPT_ENNEMY);
        return INTERRUPT_ENNEMY;
    }
    if ((etat->vie == 1) && !(pile_vide(etat->memoire))) {
        Log_info(CAT_FOURMIS, "INTERRUPT_LIFE DETECTEE\n");
        interrupt_incr(etat, INTERRUPT_LIFE);
        return INTERRUPT_LIFE;
    }
    return AUCUN_INTERRUPT;
}