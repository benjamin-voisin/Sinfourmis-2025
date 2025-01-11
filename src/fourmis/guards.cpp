#include "guards.hpp"
#include "commun.h"
#include "utils/b_constants.h"
#include "utils/utils.h"

#include <cstdint>
#include <stdio.h>
#include <assert.h>
#include <vector>

void guard_loads(fourmi_etat* etat, uint32_t team_id) {
    commun_loads(etat, team_id, nullptr, 0);
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    mem->comm.type = ANT_KIND_GUARD;
    mem->comportement = RANDOMWALK;
}

fourmi_retour guard_action(fourmi_etat *etat, const salle *salle) {
    memoire_guard_t* mem = (memoire_guard_t*) etat->memoire;
    uint8_t id;
    uint8_t direction = random_dir(etat, salle);
    switch (mem->comportement) {
    case RANDOMWALK:
      if (salle->type == EAU) {
        mem->comportement = GUARDING;
        return guard_action(etat, salle);
      }
      return commun_action_versdirection(etat, salle, direction, NO_PHEROMONE, 0);
    case GUARDING:
      uint8_t enemy_team_present = 255;
      std::vector<fourmis_compteur> compteurs(salle->compteurs_fourmis, salle->compteurs_fourmis + salle->taille_liste);
      for (fourmis_compteur compteur : compteurs) {
        if (compteur.equipe != mem->comm.team_id && compteur.nombre > 0) {
          enemy_team_present = compteur.equipe;
        }
      }
      return enemy_team_present != 255 ? commun_action_attaque(enemy_team_present) : commun_action_attendre();
    }
}


void guard_postaction(fourmi_etat *etat, const salle *salle) {

}

void guard_coportement_pp(FILE* f, enum guard_comportement comportement) {
    switch (comportement)
    {
    case GUARDING:
        fprintf(f, "GUARDING");
        break;
    case RANDOMWALK:
        fprintf(f, "RANDOMWALK");
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
    fprintf(f, "GUARD_ANT {\n");
    guard_body_pp(f, etat);
    commun_body_pp(f, etat);
    fprintf(f, "}\n");
}
