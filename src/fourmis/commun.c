#include "commun.h"

#include "stdbool.h"

void commun_postaction(fourmi_retour ret, fourmi_etat* etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    mem->eau = etat->eau;
    mem->nourriture = etat->nourriture;
    mem->vie = etat->vie;
    mem->action = ret.action;
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
    commun_action_versdirection_(etat, salle, DEPUISBASE, direction, pheromone_type, pheromone);
}

fourmi_retour commun_action_versbase(fourmi_etat* etat, const salle *salle) {
    pile_t* hd = head(etat->memoire);
    commun_action_versdirection_(etat, salle, VERSBASE, hd->direction, NO_PHEROMONE, 0);
}

void commun_feedback_deplacement(fourmi_etat* etat, const salle *salle) {
    printf("Failwith todo %s\n", "commun_feedback_deplacement");
    exit(1);
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
