#include "commun.h"

#include "stdbool.h"

void commun_postaction(fourmi_retour ret, fourmi_etat* etat, const salle *salle) {
    memoire_commun_t* mem = (memoire_commun_t*) etat->memoire;
    mem->eau = etat->eau;
    mem->nourriture = etat->nourriture;
    mem->vie = etat->vie;
    mem->action = ret.action;
}

fourmi_retour commun_action_versbase(fourmi_etat* etat, const salle *salle) {

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
