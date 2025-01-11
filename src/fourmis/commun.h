#ifndef ANT_COMMUN_H
#define ANT_COMMUN_H

#include "utils/pile.h"

#include "stdint.h"
#include "../sinfourmis.h"

#include <stdio.h>

enum communcomportement_e {
  AUCUN,
  VERSBASE,
  DEPUISBASE,
};

/*
SCHEMA MEMOIRE D'UNE FOURMIS

*******************************************************************
* PILE     * MEMOIRE  * MEMOIRE     * TAS SPE              PILE   *
* METADATA * COMMMUNE * SPECIALISEE *  ----->             <-----  *
*          *          *             *                    COMMUNE  *
*******************************************************************
*/

typedef struct memoire_commun_s {
    pilemetadata_t pilemetadata;
    uint8_t type;
    enum communcomportement_e comportement;
    uint32_t team_id;
    uint8_t vie;
    int32_t eau;
    int32_t nourriture;
    fourmi_retour ret;
} memoire_commun_t;

void commun_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size);

void commun_postaction(fourmi_etat* etat, const salle *salle);

void commun_feedback_deplacement(fourmi_etat* etat, const salle *salle);

void commun_feedback_commence_construction(fourmi_etat* etat, const salle *salle);

void commun_feedback_termine_construction(fourmi_etat* etat, const salle *salle);

void commun_feedback_attaque_tunnel(fourmi_etat* etat, const salle *salle);

fourmi_retour commun_action_verslead(fourmi_etat* etat, const salle *salle);

fourmi_retour commun_action_versbase(fourmi_etat* etat, const salle *salle);

fourmi_retour commun_action_versdirection(fourmi_etat* etat, const salle *salle, uint8_t direction, pheromone_type pheromone_type, uint8_t pheromone);

fourmi_retour commun_action_attendre_phero(pheromone_type pheromone_type, uint8_t pheromone);

fourmi_retour commun_action_attendre();

void commun_pp(FILE* f, fourmi_etat* etat);

#endif