#include "scout.h"
#include "utils/b_constants.h"
#include "utils/utils.h"

uint8_t freshid(fourmi_etat *etat) {
    memoire_scout_t* mem = etat->memoire;
    uint8_t id = (mem->id & ID_MASK) | ((mem->tile_counter++) & COUNTER_MASK);
    return id;
}

uint8_t no_more_id(fourmi_etat *etat) {
    memoire_scout_t* mem = etat->memoire;
    return (mem->tile_counter == COUNTER_MASK);
}

fourmi_retour scout_action(fourmi_etat *etat, const salle *salle) {
    memoire_scout_t* mem = etat->memoire;
    switch (mem->comportement)
    {
    case FOLLOWLEAD:
        if (!pile_complete(etat->memoire))
            return commun_action_verslead(etat, salle);
        mem->comportement = SCOUTING;
        return scout_action(etat, salle);

    case SCOUTING_NEW_TILE:
        mem->comportement = SCOUTING;
        if (salle->pheromone == 0) {
            // Il faut attribuer un ID au nouveau noeud
            uint8_t id = freshid(etat);
            pile_edit_id(etat->memoire, id);
            if (no_more_id(etat))
                mem->comportement = BACK;
            return commun_action_attendre_phero(PRIVE, id);
        }
        return scout_action(etat, salle);
        
    case SCOUTING:
        if (water2base(etat->memoire) > 1.5 * etat->eau) {
            mem->comportement = BACK;
            return scout_action(etat, salle);
        }

        uint8_t direction = random_other_dir(etat, salle);
        mem->comportement = SCOUTING_NEW_TILE;
        return commun_action_versdirection(etat, salle, direction);

    case BACK:
        if (!pile_vide(etat->memoire))
            return commun_action_versbase(etat, salle);
        mem->comportement = WAITBASE;
        return scout_action(etat, salle);

    case WAITBASE:
        return commun_action_attendre();

    default:
        printf("SCOUT ACTION UNDEFINED\n");
        exit(1);
        break;
    }
}


void scout_postaction(fourmi_etat *etat, const salle *salle) {

}
