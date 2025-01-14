#include "update.h"

void reine_update_creer_fourmi(reine_t* reine, uint8_t arg);

void reine_update_envoyer_fourmi(reine_t* reine, uint8_t arg);

void reine_update_recuperer_fourmi(reine_t* reine, uint8_t arg);

void reine_update_ameliore_stockage(reine_t* reine);

void reine_update_ameliore_production(reine_t* reine);

void reine_update_ameliore_envoi(reine_t* reine);

void reine_update_ameliore_vitesse_amelioration(reine_t* reine);

void reine_update_ameliore_ramassage(reine_t* reine);

void reine_update_ameliore_vie(reine_t* reine);

void reine_update_ameliore_eau(reine_t* reine);

void reine_update_ameliore_degats(reine_t* reine);

void reine_update_passe(reine_t* reine);

void reine_update_state(reine_t* reine, reine_retour ret) {
    switch (ret.action) {
    case CREER_FOURMI:
        return reine_update_creer_fourmi(reine, ret.arg);
    case ENVOYER_FOURMI:
        return reine_update_envoyer_fourmi(reine, ret.arg);
    case RECUPERER_FOURMI:
        return reine_update_recuperer_fourmi(reine, ret.arg);
    case AMELIORE_STOCKAGE:
        return reine_update_ameliore_stockage(reine);
    case AMELIORE_PRODUCTION:
        return reine_update_ameliore_production(reine);
    case AMELIORE_ENVOI:
        return reine_update_ameliore_envoi(reine);
    case AMELIORE_VITESSE_AMELIORATION:
        return reine_update_ameliore_vitesse_amelioration(reine);
    case AMELIORE_RAMASSAGE:
        return reine_update_ameliore_ramassage(reine);
    case AMELIORE_VIE:
        return reine_update_ameliore_vie(reine);
    case AMELIORE_EAU:
        return reine_update_ameliore_eau(reine);
    case AMELIORE_DEGATS:
        return reine_update_ameliore_degats(reine);
    case REINE_PASSE:
        return reine_update_passe(reine);
    default:
        Error(CAT_OTHER, "Action de la reine %d inconnue.", ret.action);
    }
}