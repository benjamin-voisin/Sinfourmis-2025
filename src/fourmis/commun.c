#include "commun.h"

#include "utils/b_constants.h"
#include "utils/utils.h"

#include <stdbool.h>
#include <stdint.h>

void stats_loads(fourmi_etat *etat) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  mem->eau = etat->eau;
  mem->nourriture = etat->nourriture;
  mem->vie = etat->vie;
}

void ret_loads(fourmi_etat* etat) {
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;

    mem->ret.pheromone = 0;
    mem->ret.depose_pheromone = NO_PHEROMONE;
    mem->ret.action = FOURMI_PASSE;
    mem->ret.arg = 0;
}

void commun_reloads(fourmi_etat* etat, fourmi_stats_t stats) {
    stats_loads(etat);
    ret_loads(etat);
    pile_reloads(etat->memoire);
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
    fourmi_stats_copy(&mem->stats, stats);
    mem->prevent_prehemption = false;
}

void commun_loads(fourmi_etat *etat, fourmi_stats_t stats, uint32_t team_id, pile_t *pile,
                  size_t size) {
  static uint32_t id = 0;
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  pile_loads(etat->memoire, pile, size);
  mem->team_id = team_id;
  mem->fourmis_id = id++;
  mem->type = ANT_KIND_COMMON;
  mem->comportement = AUCUN;
  mem->prevent_prehemption = false;
  fourmi_stats_copy(&mem->stats, stats);
  stats_loads(etat);
  ret_loads(etat);
}

void commun_postaction(fourmi_etat *etat, const salle *salle) {
  stats_loads(etat);
}

fourmi_retour
commun_action_versdirection_(fourmi_etat *etat, const salle *salle,
                             enum communcomportement_e comp, uint8_t dir,
                             pheromone_type pheromone_type, uint8_t pheromone) {
  if (!(dir < salle->degre)) {
      Log_warning(CAT_FOURMIS, "Direction supérieure au degré\n");
      Log_warning(CAT_FOURMIS, "Selecting random dir\n");
      dir = random_other_dir(etat, salle);
  }
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  mem->comportement = comp;

  fourmi_retour ret;
  ret.action = DEPLACEMENT;
  ret.arg = dir;
  ret.depose_pheromone = pheromone_type;
  ret.pheromone = pheromone;

  return ret;
}

fourmi_retour commun_action_versdirection(fourmi_etat *etat, const salle *salle,
                                          uint8_t direction,
                                          pheromone_type pheromone_type,
                                          uint8_t pheromone) {
  return commun_action_versdirection_(etat, salle, DEPUISBASE, direction,
                                      pheromone_type, pheromone);
}

fourmi_retour commun_action_versbase(fourmi_etat *etat, const salle *salle) {
  simplipile(etat->memoire);
  pile_t *hd = head(etat->memoire);
  return commun_action_versdirection_(etat, salle, VERSBASE, hd->degree_sortant,
                                      NO_PHEROMONE, 0);
}

fourmi_retour commun_action_verslead(fourmi_etat *etat, const salle *salle) {
  Assert(CAT_FOURMIS, !pile_complete(etat->memoire), "Vers lead sur pile complete!");
  pile_t *p = pile_get(etat->memoire, pile_size(etat->memoire));
  return commun_action_versdirection(etat, salle, p->degree_entrant,
                                     NO_PHEROMONE, 0);
}

fourmi_retour commun_action_attendre_phero(pheromone_type pheromone_type,
                                           uint8_t pheromone) {
  fourmi_retour ret;
  ret.action = FOURMI_PASSE;
  ret.arg = 0;
  ret.depose_pheromone = pheromone_type;
  ret.pheromone = pheromone;
  return ret;
}

fourmi_retour commun_action_ramasse_phero(pheromone_type pheromone_type,
                                          uint8_t pheromone) {
  fourmi_retour ret;
  ret.action = RAMASSE_NOURRITURE;
  ret.arg = 0;
  ret.depose_pheromone = pheromone_type;
  ret.pheromone = pheromone;
  return ret;
}

fourmi_retour commun_action_ramasse() {
  return commun_action_ramasse_phero(NO_PHEROMONE, 0);
}

fourmi_retour commun_action_attaque_phero(pheromone_type pheromone_type,
                                          uint8_t pheromone, uint8_t ennemy) {
  fourmi_retour ret;
  ret.action = ATTAQUE;
  ret.arg = ennemy;
  ret.depose_pheromone = pheromone_type;
  ret.pheromone = pheromone;
  return ret;
}

fourmi_retour commun_action_attaque(uint8_t ennemy) {
  return commun_action_attaque_phero(NO_PHEROMONE, 0, ennemy);
}

fourmi_retour commun_action_attendre() {
  return commun_action_attendre_phero(NO_PHEROMONE, 0);
}

void commun_feedback_deplacement(fourmi_etat *etat, const salle *salle) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  if (etat->result >= -1) {
    switch (mem->comportement) {
    case VERSBASE:
      depiler(etat->memoire);
      mem->comportement = AUCUN;
      break;
    case DEPUISBASE:
      pile_t hd;
      hd.degree_sortant = etat->result;
      hd.degree_entrant = mem->ret.arg;
      hd.id = salle->pheromone;
      hd.type = salle->type;
      hd.poid = mem->eau - etat->eau;
      empiler(etat->memoire, hd);
      mem->comportement = AUCUN;
      break;
    case AUCUN:
      Error(CAT_MAIN, "ERREUR COMPORTEMENT DE DEPLACEMENT INDEFINI\n");
    }
  } else {
    Error(CAT_TODO, "TODO: commun_feedback_deplacement res=%d\n", etat->result);
  }
}

void commun_feedback_commence_construction(fourmi_etat *etat,
                                           const salle *salle) {
  Error(CAT_TODO, "TODO: commun_feedback_commence_construction\n");
}

void commun_feedback_termine_construction(fourmi_etat *etat,
                                          const salle *salle) {
  Error(CAT_TODO, "TODO: commun_feedback_termine_construction\n");
}

void commun_feedback_attaque_tunnel(fourmi_etat *etat, const salle *salle) {
  Error(CAT_TODO, "TODO: commun_feedback_attaque_tunnel\n");
}

void common_kind_pp(logcat_t cat, loglevel_t level, uint8_t type) {
  switch (type) {
  case ANT_KIND_NEW:
    Log(cat, level, "ANT_KIND_NEW");
    break;
  case ANT_KIND_COMMON:
    Log(cat, level, "ANT_KIND_COMMON");
    break;
  case ANT_KIND_SCOUT:
    Log(cat, level, "ANT_KIND_SCOUT");
    break;
  case ANT_KIND_GUARD:
    Log(cat, level, "ANT_KIND_GUARD");
    break;
  case ANT_KIND_FOOD:
    Log(cat, level, "ANT_KIND_FOOD");
    break;
  default:
    Log(cat, level, "ANT_KIND_UNKNOWN");
    break;
  }
}

void commun_body_pp(logcat_t cat, loglevel_t level, fourmi_etat *etat) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  fourmi_stats_body_pp(cat, level, &mem->stats);
  Log(cat, level, "COMMON BODY:\n");
  Log(cat, level, "    kind       = ");
  common_kind_pp(CAT_NOBLOAT, level, mem->type);
  Log(CAT_NOBLOAT, level, "\n");
  Log(cat, level, "    team_id    = %u\n", mem->team_id);
  Log(cat, level, "    fourmi_id  = %u\n", mem->fourmis_id);
  Log(cat, level, "    vie        = %u\n", mem->vie);
  Log(cat, level, "    eau        = %u\n", mem->eau);
  Log(cat, level, "    nourriture = %u\n", mem->nourriture);
  retour_pp_body(cat, level, mem->ret);
  pile_pp(cat, level, etat->memoire);
}

void commun_pp(logcat_t cat, loglevel_t level, fourmi_etat *etat) {
  Log(cat, level, "COMMON_ANT {\n");
  commun_body_pp(cat, level, etat);
  Log(cat, level, "}\n");
}
