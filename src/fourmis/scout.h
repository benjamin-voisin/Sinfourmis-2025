#ifndef ANT_SCOUT_H
#define ANT_SCOUT_H

#include "stdint.h"
#include "commun.h"

enum scoutcomportement_e {
  SCOUTING,
  BACK2BASE
}; 

typedef struct memoire_scout_s {
    memoire_commun_t comm;

    // Représente le comportement du scout
    enum scoutcomportement_e state;

    // Représente l'identifiant unique du scout
    uint8_t id; // 5 bit integer

    // Représente les identifiants de cases attribuées par la fourmis
    uint8_t tile_counter; // 3 bit integer
} memoire_scout_t;


fourmi_retour scout_action(fourmi_etat *etat, const salle *salle);
void scout_postaction(fourmi_etat *etat, const salle *salle);

#endif