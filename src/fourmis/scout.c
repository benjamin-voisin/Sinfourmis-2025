#include "scout.h"
#include "utils/b_constants.h"
#include "utils/utils.h"
#include "utils/log.h"
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

void scout_reloads(fourmi_etat* etat) {
    commun_reloads(etat);
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    mem->comportement = FOLLOWLEAD;
    pile_reduceloads(etat->memoire);
}

void scout_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size, uint32_t id) {
    static uint8_t fresh_id = 0;
    Assert(CAT_FOURMIS, fresh_id < 0b111111, "Scout ID Overflow\n");
    commun_loads(etat, team_id, pile, size);
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    mem->comm.type = ANT_KIND_SCOUT;
    mem->comportement = FOLLOWLEAD;
    mem->id = (fresh_id++) << 2;
    mem->tile_counter = 0;
    if (mem->id == 0)
        mem->tile_counter++;
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
            if (salle->type == NOURRITURE)
                return commun_action_ramasse_phero(PRIVE, id);
            return commun_action_attendre_phero(PRIVE, id);
        }
        if (salle->type == NOURRITURE)
            return commun_action_ramasse();
        return scout_action(etat, salle);
        
    case SCOUTING:
        /* 
        if (water2base(etat->memoire) > 1.5 * etat->eau) {
            mem->comportement = BACK;
            return scout_action(etat, salle);
        } */

        direction = salle->public_pheromone % salle->degre;

        mem->comportement = SCOUTING_NEW_TILE;
        return commun_action_versdirection(etat, salle, direction, PUBLIC, salle->public_pheromone+1);

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
        Log_warning(CAT_FOURMIS, "SCOUT ACTION UNDEFINED\n");
        Log_warning(CAT_FOURMIS, "Fallback BACK\n");
        mem->comportement = BACK;
        return scout_action(etat, salle);
    }
}


void scout_postaction(fourmi_etat *etat, const salle *salle) {

}

void scout_coportement_pp(logcat_t cat, loglevel_t level, enum scoutcomportement_e comportement) {
    switch (comportement)
    {
    case FOLLOWLEAD:
        Log(cat, level, "FOLLOWLEAD");
        break;
    case SCOUTING:
        Log(cat, level, "SCOUTING");
        break;
    case SCOUTING_NEW_TILE:
        Log(cat, level, "SCOUTING_NEW_TILE");
        break;
    case BACK:
        Log(cat, level, "BACK");
        break;
    case BACKWATER:
        Log(cat, level, "BACKWATER");
        break;
    case WAITWATER:
        Log(cat, level, "WAITWATER");
        break;
    case WAITBASE:
        Log(cat, level, "WAITBASE");
        break;
    default:
        Log(cat, level, "UNKNOWN");
        break;
    }
}

void scout_body_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    Log(cat, level, "SCOUT BODY:\n");
    Log(cat, level, "    status     = ");
    scout_coportement_pp(CAT_NOBLOAT, level, mem->comportement);
    Log(CAT_NOBLOAT, level, "\n");
    Log(cat, level, "    scout_id   = %u\n", mem->id);
    Log(cat, level, "    tile_count = %u\n", mem->tile_counter);
}

void scout_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat) {
    Log(cat, level, "SCOUT_ANT {\n");
    scout_body_pp(cat, level, etat);
    commun_body_pp(cat, level, etat);
    Log(cat, level, "}\n");
}