#include "interrupts.h"
#include "pile.h"
#include "../../constants.hpp"

enum fourminterrupt_e interrupt(fourmi_etat *etat, const salle *salle) {
    if ((etat->eau < 19) && !(pile_vide(etat->memoire)))
        return INTERRUPT_WATER;
    if (salle->taille_liste > 1)
        return INTERRUPT_ENNEMY;
    if ((etat->vie < DEFAULT_MAX_LIFE) && !(pile_vide(etat->memoire)))
        return INTERRUPT_LIFE;
    return AUCUN_INTERRUPT;
}