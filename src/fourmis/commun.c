#include "commun.h"

#include "utils/b_constants.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

void stats_loads(fourmi_etat* etat) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    mem->eau = etat->eau;
    mem->nourriture = etat->nourriture;
    mem->vie = etat->vie;
}

void commun_loads(fourmi_etat* etat, pile_t* pile, size_t size) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    pile_loads(etat->memoire, pile, size);
    mem->type = ANT_KIND_COMMON;
    mem->comportement = AUCUN;
    stats_loads(etat);
    fourmi_retour ret = {0};
}

void commun_postaction(fourmi_etat* etat, const salle *salle) {
    stats_loads(etat);
}

fourmi_retour commun_action_versdirection_(fourmi_etat* etat, const salle *salle,
    enum communcomportement_e comp, uint8_t dir,
    pheromone_type pheromone_type, uint8_t pheromone) {
    assert(dir < salle->degre);
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    mem->comportement = comp;
    
    fourmi_retour ret;
    ret.action = DEPLACEMENT;
    ret.arg = dir;
    ret.depose_pheromone = pheromone_type;
    ret.pheromone = pheromone;

    return ret;
}

fourmi_retour commun_action_versdirection(fourmi_etat* etat, const salle *salle, uint8_t direction,
    pheromone_type pheromone_type, uint8_t pheromone) {
    return commun_action_versdirection_(etat, salle, DEPUISBASE, direction, pheromone_type, pheromone);
}

fourmi_retour commun_action_versbase(fourmi_etat* etat, const salle *salle) {
    pile_t* hd = head(etat->memoire);
    return commun_action_versdirection_(etat, salle, VERSBASE, hd->degree_sortant, NO_PHEROMONE, 0);
}

fourmi_retour commun_action_verslead(fourmi_etat* etat, const salle *salle) {
    false_empiler(etat->memoire);
    pile_t* p = head(etat->memoire);
    return commun_action_versdirection(etat, salle, p->degree_sortant, NO_PHEROMONE, 0);
}

fourmi_retour commun_action_attendre_phero(pheromone_type pheromone_type, uint8_t pheromone) {
    fourmi_retour ret;
    ret.action = FOURMI_PASSE;
    ret.arg = 0;
    ret.depose_pheromone = pheromone_type;
    ret.pheromone = pheromone;
    return ret;
}

fourmi_retour commun_action_attendre() {
    return commun_action_attendre_phero(NO_PHEROMONE, 0);
}

void commun_feedback_deplacement(fourmi_etat* etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    if (etat->result > 0) {
        switch (mem->comportement) {
            case VERSBASE:
                depiler(etat->memoire);
            case DEPUISBASE:
                pile_t hd;
                hd.degree_sortant = etat->result;
                hd.degree_entrant = mem->ret.arg;
                hd.id = salle->pheromone;
                hd.type = salle->type;
                hd.poid = mem->eau - etat->eau;
                empiler(etat->memoire, hd);
            case AUCUN:
                printf("ERREUR COMPORTEMENT DE DEPLACEMENT INDEFINI");
                exit(1);
        }
    } else {
        printf("Failwith todo %s\n", "commun_feedback_deplacement FAILURE");
        exit(1);
    }
}

void commun_feedback_commence_construction(fourmi_etat* etat, const salle *salle) {
    printf("Failwith todo %s\n", "commun_feedback_commence_construction");
    exit(1);
}

void commun_feedback_termine_construction(fourmi_etat* etat, const salle *salle) {
    printf("Failwith todo %s\n", "commun_feedback_termine_construction");
    exit(1);
}

void commun_feedback_attaque_tunnel(fourmi_etat* etat, const salle *salle) {
    printf("Failwith todo %s\n", "commun_feedback_attaque_tunnel");
    exit(1);
}
