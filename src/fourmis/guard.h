#pragma once

#include "commun.h"
#include <stdint.h>

enum guardcomportement_e {
  G_FOLLOWLEAD,
  G_GUARD,
  G_BACK,
  G_WAITBASE
}; 

typedef struct __attribute__((__packed__)) memoire_guard_s {
    memoire_commun_t comm;

    enum guardcomportement_e comportement;
} memoire_guard_t;

void guard_loads(fourmi_etat* etat, fourmi_stats_t stats, uint32_t team_id, pile_t* pile, size_t size);
void guard_reloads(fourmi_etat* etat, fourmi_stats_t stats);

fourmi_retour guard_action(fourmi_etat *etat, const salle *salle);

void guard_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat);
