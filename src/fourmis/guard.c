#include "guard.h"
#include "utils/b_constants.h"
#include "../constants.hpp"

#include <stdio.h>
#include <assert.h>

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
        assert(salle->type == EAU);
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
        printf("GUARD ACTION UNDEFINED\n");
        exit(1);
        break;
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
