#include "food.h"
#include "utils/b_constants.h"
#include "utils/log.h"

#include <stdio.h>

void food_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size) {
    commun_loads(etat, team_id, pile, size);
    memoire_food_t* mem = (memoire_food_t*) etat->memoire;
    mem->comm.type = ANT_KIND_FOOD;
    mem->comportement = F_FOLLOWLEAD;
}

void food_reloads(fourmi_etat* etat) {
    commun_reloads(etat);
    memoire_food_t* mem = (memoire_food_t*) etat->memoire;
    mem->comportement = F_FOLLOWLEAD;
}

fourmi_retour food_action(fourmi_etat *etat, const salle *salle) {
    memoire_food_t* mem = (memoire_food_t*) etat->memoire;
    uint8_t id;
    uint8_t direction;
    switch (mem->comportement) {
    case F_FOLLOWLEAD:
        if (salle->type == NOURRITURE) {
            if (!pile_complete(etat->memoire))
                Log_info(CAT_FOURMIS, "La ramasseuse à trouvé de la Bouffe avant sa destination !\n"); 
        
            mem->comportement = F_BACK;
            return commun_action_ramasse();
        }
        
        if (!pile_complete(etat->memoire))
            return commun_action_verslead(etat, salle);
        Log_warning(CAT_FOURMIS, "La ramasseur n'est pas arrivée sur une case de bouffe\n");
        Log_warning(CAT_FOURMIS, "Retour à la reine\n");
        mem->comportement = F_BACK;
        return food_action(etat, salle);
        
 
    case F_BACK:
        if (!pile_vide(etat->memoire))
            return commun_action_versbase(etat, salle);
        mem->comportement = F_WAITBASE;
        return food_action(etat, salle);

    case F_WAITBASE:
        return commun_action_attendre();

    default:
        Log_warning(CAT_FOURMIS, "FOOD ACTION UNDEFINED\n");
        Log_warning(CAT_FOURMIS, "Fallback F_BACK\n");
        mem->comportement = F_BACK;
        return food_action(etat, salle);
    }
}

void food_coportement_pp(logcat_t cat, loglevel_t level, enum foodcomportement_e comportement) {
    switch (comportement)
    {
    case F_FOLLOWLEAD:
        Log(cat, level, "FOLLOWLEAD");
        break;
    case F_BACK:
        Log(cat, level, "BACK");
        break;
    case F_WAITBASE:
        Log(cat, level, "WAITBASE");
        break;
    default:
        Log(cat, level, "UNKNOWN");
        break;
    }
}

void food_body_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat) {
    memoire_food_t* mem = (memoire_food_t*) etat->memoire;
    Log(cat, level, "FOOD BODY:\n");
    Log(cat, level, "    status     = ");
    food_coportement_pp(CAT_NOBLOAT, level, mem->comportement);
    Log(CAT_NOBLOAT, level, "\n");
}

void food_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat) {
    Log(cat, level, "FOOD_ANT {\n");
    food_body_pp(cat, level, etat);
    commun_body_pp(cat, level, etat);
    Log(cat, level, "}\n");
}
