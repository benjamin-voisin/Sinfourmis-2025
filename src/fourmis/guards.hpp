#pragma once

#include "stdint.h"
#include "commun.h"

enum guard_comportement {
  RANDOMWALK,
  GUARDING,
}; 

typedef struct memoire_guard_s {
    memoire_commun_t comm;

    // Représente le comportement du scout
    enum guard_comportement comportement;
} memoire_guard_t;

void guard_loads(fourmi_etat* etat, uint32_t team_id);

fourmi_retour guard_action(fourmi_etat *etat, const salle *salle);
void guard_postaction(fourmi_etat *etat, const salle *salle);

void guard_pp(FILE* f, fourmi_etat* etat);
