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

void retour_pp_action(logcat_t cat, loglevel_t level, fourmi_action act) {
    switch (act)
    {
    case DEPLACEMENT:
        Log(cat, level, "DEPLACEMENT");
        break;
    case RAMASSE_NOURRITURE:
        Log(cat, level, "RAMASSE_NOURRITURE");
        break;
    case COMMENCE_CONSTRUCTION:
        Log(cat, level, "COMMENCE_CONSTRUCTION");
        break;
    case TERMINE_CONSTRUCTION:
        Log(cat, level, "TERMINE_CONSTRUCTION");
        break;
    case ATTAQUE:
        Log(cat, level, "ATTAQUE");
        break;
    case ATTAQUE_TUNNEL:
        Log(cat, level, "ATTAQUE_TUNNEL");
        break;
    case FOURMI_PASSE:
        Log(cat, level, "FOURMI_PASSE");
        break;    
    default:
        Log(cat, level, "UNKNOWN");
        break;
    }
}

void retour_pp_phero(logcat_t cat, loglevel_t level, pheromone_type t) {
    switch (t)
    {
    case NO_PHEROMONE:
        Log(cat, level, "NO_PHEROMONE");
        break;
    case PRIVE:
        Log(cat, level, "PRIVE");
        break;
    case PUBLIC:
        Log(cat, level, "PUBLIC");
        break;     
    default:
        Log(cat, level, "UNKNOWN");
        break;
    }
}

void retour_pp_body(logcat_t cat, loglevel_t level, fourmi_retour ret) {
    Log(cat, level, "RET:\n");
    Log(cat, level, "    action     = ");
    retour_pp_action(CAT_NOBLOAT, level, ret.action);
    Log(CAT_NOBLOAT, level, "\n");
    Log(cat, level, "    arg        = %u\n", ret.arg);
    Log(cat, level, "    fero_type  = ", ret.depose_pheromone);
    retour_pp_phero(CAT_NOBLOAT, level, ret.depose_pheromone);
    Log(CAT_NOBLOAT, level, "\n");
    Log(cat, level, "    fero       = %u\n", ret.pheromone);
}

void retour_pp(logcat_t cat, loglevel_t level, fourmi_retour ret) {
    Log(cat, level, "ACTION {\n");
    retour_pp_body(cat, level, ret);
    Log(cat, level, "}\n");
}