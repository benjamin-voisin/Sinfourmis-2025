#ifndef ANT_COMMUN_H
#define ANT_COMMUN_H

#include "utils/pile.h"
#include "../utils/stats/fourmi_stats.h"
#include "../utils/log.h"
#include "utils/interrupts.h"

#include "../sinfourmis.h"

#include <stdint.h>
#include <stdbool.h>


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

typedef struct __attribute__((__packed__)) memoire_commun_s {
    pilemetadata_t pilemetadata;
    uint8_t type;
    fourmi_stats_t stats;
    interruptstats_t interrupt_stats;
    enum communcomportement_e comportement;
    uint32_t team_id;
    uint32_t fourmis_id;
    uint8_t vie;
    int32_t eau;
    int32_t nourriture;
    bool prevent_prehemption;
    fourmi_retour ret;
} memoire_commun_t;

void commun_loads(fourmi_etat* etat, fourmi_stats_t stats, uint32_t team_id, pile_t* pile, size_t size);

void commun_reloads(fourmi_etat* etat, fourmi_stats_t stats);

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

fourmi_retour commun_action_attaque(uint8_t team_id);

fourmi_retour commun_action_ramasse_phero(pheromone_type pheromone_type, uint8_t pheromone);

fourmi_retour commun_action_ramasse();

fourmi_retour commun_action_attaque_phero(pheromone_type pheromone_type, uint8_t pheromone, uint8_t ennemy);

fourmi_retour commun_action_attaque(uint8_t ennemy);

void commun_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat);

void commun_body_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat);

void common_kind_pp(logcat_t cat, loglevel_t level, uint8_t type);

#endif
