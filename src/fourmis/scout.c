#include "scout.h"
#include "utils/b_constants.h"
#include "utils/utils.h"
#include "../constants.hpp"

#include <stdio.h>
#include <assert.h>

uint8_t freshid(fourmi_etat *etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    uint8_t id = (mem->id & ID_MASK) | ((mem->tile_counter++) & COUNTER_MASK);
    return id;
}

uint8_t no_more_id(fourmi_etat *etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    return (mem->tile_counter == COUNTER_MASK);
}


void scout_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size, uint32_t id) {
    assert(id > 0);
    commun_loads(etat, team_id, pile, size);
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    mem->comm.type = ANT_KIND_SCOUT;
    mem->comportement = FOLLOWLEAD;
    mem->id = id;
    mem->tile_counter = 0;
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
            return commun_action_ramasse_phero(PRIVE, id);
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

void scout_coportement_pp(FILE* f, enum scoutcomportement_e comportement) {
    switch (comportement)
    {
    case FOLLOWLEAD:
        fprintf(f, "FOLLOWLEAD");
        break;
    case SCOUTING:
        fprintf(f, "SCOUTING");
        break;
    case SCOUTING_NEW_TILE:
        fprintf(f, "SCOUTING_NEW_TILE");
        break;
    case BACK:
        fprintf(f, "BACK");
        break;
    case BACKWATER:
        fprintf(f, "BACKWATER");
        break;
    case WAITWATER:
        fprintf(f, "WAITWATER");
        break;
    case WAITBASE:
        fprintf(f, "WAITBASE");
        break;
    default:
        fprintf(f, "UNKNOWN");
        break;
    }
}

void scout_body_pp(FILE* f, fourmi_etat* etat) {
    memoire_scout_t* mem = (memoire_scout_t*) etat->memoire;
    fprintf(f, "SCOUT BODY:\n");
    fprintf(f, "    status     = ");
    scout_coportement_pp(f, mem->comportement);
    fprintf(f, "\n");
    fprintf(f, "    scout_id   = %u\n", mem->id);
    fprintf(f, "    tile_count = %u\n", mem->tile_counter);
}

void scout_pp(FILE* f, fourmi_etat* etat) {
    fprintf(f, "SCOUT_ANT {\n");
    scout_body_pp(f, etat);
    commun_body_pp(f, etat);
    fprintf(f, "}\n");
}