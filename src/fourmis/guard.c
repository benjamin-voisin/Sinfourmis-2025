#include "guard.h"
#include "utils/b_constants.h"
#include "utils/log.h"
#include "../constants.hpp"

#include <stdio.h>

void guard_loads(fourmi_etat* etat, uint32_t team_id, pile_t* pile, size_t size) {
    commun_loads(etat, team_id, pile, size);
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    mem->comm.type = ANT_KIND_GUARD;
    mem->comportement = G_FOLLOWLEAD;
}

void guard_reloads(fourmi_etat* etat) {
    commun_reloads(etat);
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    mem->comportement = G_FOLLOWLEAD;
}

fourmi_retour guard_action(fourmi_etat *etat, const salle *salle) {
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    uint8_t id;
    uint8_t direction;
    switch (mem->comportement) {
    case G_FOLLOWLEAD:
        if (!pile_complete(etat->memoire))
            return commun_action_verslead(etat, salle);
        if (salle->type != EAU) {
            Log_warning(CAT_FOURMIS, "La gardienne n'est pas arrivée sur une case EAU");
            Log_warning(CAT_FOURMIS, "Retour à la reine");
            mem->comportement = G_BACK;
            return guard_action(etat, salle);
        }

        mem->comportement = G_GUARD;
        return guard_action(etat, salle);
    
    case G_GUARD:
        if (etat->vie < DEFAULT_MAX_LIFE) {
            mem->comportement = G_BACK; 
            return guard_action(etat, salle);
        }
        return commun_action_attendre();

    case G_BACK:
        if (!pile_vide(etat->memoire))
            return commun_action_versbase(etat, salle);
        mem->comportement = G_WAITBASE;
        return guard_action(etat, salle);

    case G_WAITBASE:
        return commun_action_attendre();

    default:
        Log_warning(CAT_FOURMIS, "GUARD ACTION UNDEFINED");
        Log_warning(CAT_FOURMIS, "Fallback G_BACK");
        mem->comportement = G_BACK;
        return guard_action(etat, salle);
    }
}

void guard_coportement_pp(FILE* f, enum guardcomportement_e comportement) {
    switch (comportement)
    {
    case G_FOLLOWLEAD:
        fprintf(f, "FOLLOWLEAD");
        break;
    case G_GUARD:
        fprintf(f, "BACK");
        break;
    case G_BACK:
        fprintf(f, "WAITBASE");
        break;
    default:
        fprintf(f, "UNKNOWN");
        break;
    }
}

void guard_body_pp(FILE* f, fourmi_etat* etat) {
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    fprintf(f, "GUARD BODY:\n");
    fprintf(f, "    status     = ");
    guard_coportement_pp(f, mem->comportement);
    fprintf(f, "\n");
}

void guard_pp(FILE* f, fourmi_etat* etat) {
    fprintf(f, "FOOD_ANT {\n");
    guard_body_pp(f, etat);
    commun_body_pp(f, etat);
    fprintf(f, "}\n");
}
