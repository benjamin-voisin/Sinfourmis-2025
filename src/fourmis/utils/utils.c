#include "utils.h"

#include "pile.h"

uint8_t random_dir(fourmi_etat *etat, const salle *salle) {
    int r = rand();
    uint8_t dir = r % (salle->degre);
    return dir;
}

uint8_t random_other_dir(fourmi_etat *etat, const salle *salle) {
    uint8_t dir = random_dir(etat, salle);
    if (!pile_vide(etat->memoire)) {
        pile_t* p = head(etat->memoire);
        if (dir >= p->degree_sortant)
            dir = ((dir + 1) % salle->degre);
    }
    return dir;
}

void retour_pp_action(FILE* f, fourmi_action act) {
    switch (act)
    {
    case DEPLACEMENT:
        fprintf(f, "DEPLACEMENT");
        break;
    case RAMASSE_NOURRITURE:
        fprintf(f, "RAMASSE_NOURRITURE");
        break;
    case COMMENCE_CONSTRUCTION:
        fprintf(f, "COMMENCE_CONSTRUCTION");
        break;
    case TERMINE_CONSTRUCTION:
        fprintf(f, "TERMINE_CONSTRUCTION");
        break;
    case ATTAQUE:
        fprintf(f, "ATTAQUE");
        break;
    case ATTAQUE_TUNNEL:
        fprintf(f, "ATTAQUE_TUNNEL");
        break;
    case FOURMI_PASSE:
        fprintf(f, "FOURMI_PASSE");
        break;    
    default:
        fprintf(f, "UNKNOWN");
        break;
    }
}

void retour_pp_phero(FILE* f, pheromone_type t) {
    switch (t)
    {
    case NO_PHEROMONE:
        fprintf(f, "NO_PHEROMONE");
        break;
    case PRIVE:
        fprintf(f, "PRIVE");
        break;
    case PUBLIC:
        fprintf(f, "PUBLIC");
        break;     
    default:
        fprintf(f, "UNKNOWN");
        break;
    }
}

void retour_pp_body(FILE* f, fourmi_retour ret) {
    fprintf(f, "RET:\n");
    fprintf(f, "    action     = ");
    retour_pp_action(f, ret.action);
    fprintf(f, "\n");
    fprintf(f, "    arg        = %u\n", ret.arg);
    fprintf(f, "    fero_type  = ", ret.depose_pheromone);
    retour_pp_phero(f, ret.depose_pheromone);
    fprintf(f, "\n");
    fprintf(f, "    fero       = %u\n", ret.pheromone);
}

void retour_pp(FILE* f, fourmi_retour ret) {
    fprintf(f, "ACTION {\n");
    retour_pp_body(f, ret);
    fprintf(f, "}\n");
}