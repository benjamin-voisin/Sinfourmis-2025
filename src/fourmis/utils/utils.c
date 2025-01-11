#include "utils.h"

#include "pile.h"

uint8_t random_dir(fourmi_etat *etat, const salle *salle) {
    int r = rand();
    uint8_t dir = r % (salle->degre - 1);
    return dir;
}

uint8_t random_other_dir(fourmi_etat *etat, const salle *salle) {
    uint8_t dir = random_dir(etat, salle);
    if (!pile_vide(etat->memoire)) {
        pile_t* p = head(etat->memoire);
        if (dir >= p->degree_sortant)
            dir = ((dir + 1) % salle->degre);
    }
    return dir;
}