#ifndef ANT_COMMUN_H
#define ANT_COMMUN_H

#include "stdint.h"
#include "../sinfourmis.h"

enum communcomportement_e {
  VERSBASE,
  DEPUISBASE
}; 

typedef struct memoire_commun_s {
    uint8_t type;
    enum communcomportement_e comportement;
    uint8_t vie;
    int32_t eau;
    int32_t nourriture;
    fourmi_action action;
    uint32_t allies;
} memoire_commun_t;

void commun_postaction(fourmi_retour ret, fourmi_etat* etat, const salle *salle);

void commun_feedback_deplacement(fourmi_etat* etat, const salle *salle);

void commun_feedback_commence_construction(fourmi_etat* etat, const salle *salle);

void commun_feedback_termine_construction(fourmi_etat* etat, const salle *salle);

void commun_feedback_attaque_tunnel(fourmi_etat* etat, const salle *salle);

#endif