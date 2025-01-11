#pragma once

#include "commun.h"
#include <stdint.h>

enum foodcomportement_e {
  F_FOLLOWLEAD,
  F_BACK,
  F_WAITBASE
}; 

typedef struct memoire_food_s {
    memoire_commun_t comm;

    // Représente le comportement du food carrier
    enum foodcomportement_e comportement;
} memoire_food_t;

void food_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size);

fourmi_retour food_action(fourmi_etat *etat, const salle *salle);

void food_pp(FILE* f, fourmi_etat* etat);
