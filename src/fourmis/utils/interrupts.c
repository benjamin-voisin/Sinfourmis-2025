#include "interrupts.h"

enum fourminterrupt_e interrupt(fourmi_etat *etat, const salle *salle) {
    if (salle->taille_liste > 1)
        return INTERRUPT_ENNEMY;
    return AUCUN_INTERRUPT;
}