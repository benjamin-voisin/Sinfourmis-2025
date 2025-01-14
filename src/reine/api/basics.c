#include "basics.h"

#include <assert.h>

reine_retour reine_action_appel(reine_action act, uint8_t arg) {
    reine_retour ret;
    assert(act <= REINE_PASSE);
    ret.action = act;
    ret.arg = arg;
    return ret;
}

reine_retour reine_action_creer_fourmi(uint8_t arg) {
    return reine_action_appel(CREER_FOURMI, arg);
}

reine_retour reine_action_envoyer_fourmi(uint8_t arg) {
    return reine_action_appel(ENVOYER_FOURMI, arg);
}

reine_retour reine_action_recuperer_fourmi(uint8_t arg) {
    return reine_action_appel(RECUPERER_FOURMI, arg);
}

reine_retour reine_action_ameliore_stockage() {
    return reine_action_appel(AMELIORE_STOCKAGE, 0);
}

reine_retour reine_action_ameliore_production() {
    return reine_action_appel(AMELIORE_PRODUCTION, 0);
}

reine_retour reine_action_ameliore_envoi() {
    return reine_action_appel(AMELIORE_ENVOI, 0);
}

reine_retour reine_action_ameliore_vitesse_amelioration() {
    return reine_action_appel(AMELIORE_VITESSE_AMELIORATION, 0);
}

reine_retour reine_action_ameliore_ramassage() {
    return reine_action_appel(AMELIORE_RAMASSAGE, 0);
}

reine_retour reine_action_ameliore_vie() {
    return reine_action_appel(AMELIORE_VIE, 0);
}

reine_retour reine_action_ameliore_eau() {
    return reine_action_appel(AMELIORE_EAU, 0);
}

reine_retour reine_action_ameliore_degats() {
    return reine_action_appel(AMELIORE_DEGATS, 0);
}

reine_retour reine_action_passe() {
    return reine_action_appel(REINE_PASSE, 0);
}   