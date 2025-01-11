#include "main.h"
#include "commun.h"
#include "scout.h"
#include "utils/b_constants.h"
#include "utils/utils.h"

#include <assert.h>

void new_pp(FILE* f, fourmi_etat* etat) {
    fprintf(f, "NEW_ANT {\n");
    commun_body_pp(f, etat);
    fprintf(f, "}\n");
}

void fourmi_pp(FILE* f, fourmi_etat* etat) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    switch (mem->type) {
    case ANT_KIND_SCOUT:
        scout_pp(f, etat);
        break;
    case ANT_KIND_COMMON:
        commun_pp(f, etat);
        break;
    case ANT_KIND_NEW:
        new_pp(f, etat);
        break;
    default:
        fprintf(f, "UNKNOWN_ANT\n");
        break;
    }
}

void fourmi_feedback(fourmi_etat *etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    switch (mem->type) {
    case ANT_KIND_SCOUT:
        switch (mem->ret.action) {
        case DEPLACEMENT:
            commun_feedback_deplacement(etat, salle);
            break;
        case RAMASSE_NOURRITURE:
            assert(etat->result >= 0);
            break;
        case COMMENCE_CONSTRUCTION:
            commun_feedback_commence_construction(etat, salle);
            break;
        case TERMINE_CONSTRUCTION:
            commun_feedback_termine_construction(etat, salle);
            break;
        case ATTAQUE:
            assert(etat->result > 0);
            break;
        case ATTAQUE_TUNNEL:
            commun_feedback_attaque_tunnel(etat, salle);
            break;
        case FOURMI_PASSE:
            break;
        default:
            printf("FOURMIS ACTION UNKNOWN\n");
            exit(1);
        }   
    case ANT_KIND_NEW:
        printf("FOURMIS TYPE ANT_KIND_NEW NO FEEDBACK\n");
        exit(1);
    default:
        printf("FOURMIS TYPE ANT_KIND_NEW NO FEEDBACK\n");
        exit(1);
    }
}

fourmi_retour fourmi_act(fourmi_etat *etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    switch (mem->type) {
    case ANT_KIND_SCOUT:
        return scout_action(etat, salle);
    case ANT_KIND_NEW:
        printf("FOURMIS TYPE ANT_KIND_NEW CHERCHE ACTION\n");
        exit(1);
    default:
        printf("FOURMIS TYPE UNKNOWN CHERCHE ACTION\n");
        exit(1);
    }
}

void fourmi_postaction(fourmi_retour ret, fourmi_etat *etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    switch (mem->type) {
    case ANT_KIND_SCOUT:
        scout_postaction(etat, salle);
        break;
    case ANT_KIND_NEW:
        printf("FOURMIS TYPE ANT_KIND_NEW CHERCHE POSTACTION\n");
		break;
    default:
        printf("FOURMIS TYPE UNKNOWN CHERCHE POSTACTION\n");
        exit(1);
    }
    commun_postaction(etat, salle);
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
    fourmi_pp(stdout, etat);
    fourmi_feedback(etat, salle);
    fourmi_retour ret = fourmi_act(etat, salle);
    fourmi_postaction(ret, etat, salle);
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    mem->ret = ret;
    retour_pp(stdout, ret);
    return ret;
}



