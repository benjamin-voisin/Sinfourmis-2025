#include "interrupts.h"
#include "pile.h"

enum fourminterrupt_e interrupt(fourmi_etat *etat, const salle *salle) {
    if ((etat->eau < 19) && !(pile_vide(etat->memoire)))
        return INTERRUPT_WATER;
    if (salle->taille_liste > 1)
        return INTERRUPT_ENNEMY;
    return AUCUN_INTERRUPT;
}