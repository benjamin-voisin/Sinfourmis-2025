#pragma once

#include "commun.h"
#include <stdint.h>

enum guardcomportement_e {
  G_FOLLOWLEAD,
  G_GUARD,
  G_BACK,
  G_WAITBASE
}; 

typedef struct memoire_guard_s {
    memoire_commun_t comm;

    enum guardcomportement_e comportement;
} memoire_guard_t;

void guard_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size);

fourmi_retour guard_action(fourmi_etat *etat, const salle *salle);

void guard_pp(FILE* f, fourmi_etat* etat);
