#include "scout.h"
#include "utils/b_constants.h"
#include "utils/utils.h"
#include "../constants.hpp"

#include <stdio.h>

uint8_t freshid(fourmi_etat *etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    uint8_t id = (mem->id & ID_MASK) | ((mem->tile_counter++) & COUNTER_MASK);
    return id;
}

uint8_t no_more_id(fourmi_etat *etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    return (mem->tile_counter == COUNTER_MASK);
}

fourmi_retour scout_action(fourmi_etat *etat, const salle *salle) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    uint8_t id;
    uint8_t direction;
    switch (mem->comportement) {
    case FOLLOWLEAD:
        if (!pile_complete(etat->memoire))
            return commun_action_verslead(etat, salle);
        mem->comportement = SCOUTING;
        return scout_action(etat, salle);

    case SCOUTING_NEW_TILE:
        mem->comportement = SCOUTING;
        if (salle->pheromone == 0) {
            // Il faut attribuer un ID au nouveau noeud
            id = freshid(etat);
            pile_edit_id(etat->memoire, id);
            if (no_more_id(etat))
                mem->comportement = BACK;
            if (salle->type != VIDE)
                mem->comportement = BACK;
            return commun_action_attendre_phero(PRIVE, id);
        }
        return scout_action(etat, salle);
        
    case SCOUTING:
        if (water2base(etat->memoire) > 1.5 * etat->eau) {
            mem->comportement = BACK;
            return scout_action(etat, salle);
        }

        direction = random_other_dir(etat, salle);
        mem->comportement = SCOUTING_NEW_TILE;
        return commun_action_versdirection(etat, salle, direction, NO_PHEROMONE, 0);

    case BACKWATER:
        if (!pile_vide(etat->memoire) && (salle->type != EAU))
            return commun_action_versbase(etat, salle);
        mem->comportement = WAITWATER;
        return scout_action(etat, salle);

    case BACK:
        if (!pile_vide(etat->memoire))
            return commun_action_versbase(etat, salle);
        mem->comportement = WAITBASE;
        return scout_action(etat, salle);

    case WAITBASE:
        return commun_action_attendre();

    case WAITWATER:
        if (etat->eau < DEFAULT_MAX_WATER)
            return commun_action_attendre();
        mem->comportement = SCOUTING;
        return scout_action(etat, salle);

    default:
        printf("SCOUT ACTION UNDEFINED\n");
        exit(1);
        break;
    }
}


void scout_postaction(fourmi_etat *etat, const salle *salle) {

}
