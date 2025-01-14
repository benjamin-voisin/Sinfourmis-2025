#include "main.h"
#include "../sinfourmis.h"
#include "commun.h"
#include "scout.h"
#include "food.h"
#include "guard.h"
#include "utils/b_constants.h"
#include "utils/utils.h"
#include "utils/interrupts.h"

void new_pp(logcat_t cat, loglevel_t level, fourmi_etat *etat) {
  Log(cat, level, "NEW_ANT {\n");
  commun_body_pp(cat, level, etat);
  Log(cat, level, "}\n");
}

void fourmi_pp(logcat_t cat, loglevel_t level, fourmi_etat *etat) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  switch (mem->type) {
  case ANT_KIND_FOOD:
    food_pp(cat, level, etat);
    break;
  case ANT_KIND_SCOUT:
    scout_pp(cat, level, etat);
    break;
  case ANT_KIND_COMMON:
    commun_pp(cat, level, etat);
    break;
  case ANT_KIND_GUARD:
    guard_pp(cat, level, etat);
    break;
  case ANT_KIND_NEW:
    new_pp(cat, level, etat);
    break;
  default:
    Log_warning(CAT_MAIN, "PP UNKNOWN_ANT");
    break;
  }
}

uint8_t	fourmi_kind(fourmi_etat* etat) {
	memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
	return mem->type;
}

void fourmi_feedback(fourmi_etat *etat, const salle *salle) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  switch (mem->type) {
  case ANT_KIND_SCOUT:
  case ANT_KIND_FOOD:
  case ANT_KIND_GUARD:
    switch (mem->ret.action) {
    case DEPLACEMENT:
      commun_feedback_deplacement(etat, salle);
      break; 
    case RAMASSE_NOURRITURE:
      Assert(CAT_MAIN, etat->result >= 0, "Feedback Fail RamasseNourriture\n");
      break;
    case COMMENCE_CONSTRUCTION:
      commun_feedback_commence_construction(etat, salle);
      break;
    case TERMINE_CONSTRUCTION:
      commun_feedback_termine_construction(etat, salle);
      break;
    case ATTAQUE:
      if (etat->result <= 0)
      Log_warning(CAT_MAIN, "Feedback Fail Attaque\n");
      break;
    case ATTAQUE_TUNNEL:
      commun_feedback_attaque_tunnel(etat, salle);
      break;
    case FOURMI_PASSE:
      break;
    default:
      Error(CAT_MAIN, "FOURMIS ACTION UNKNOWN\n");
    }
    break;
  case ANT_KIND_NEW:
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW NO FEEDBACK\n");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN NO FEEDBACK\n");
  }
}

fourmi_retour fourmi_interrupt(fourmi_etat *etat, const salle *salle, enum fourminterrupt_e inter) {
	memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
	switch (inter)
	{
	case INTERRUPT_WATER:
	case INTERRUPT_LIFE:
    Log_info(CAT_FOURMIS, "INTERRUPT RESULT DIRECTION BASE\n");
		return commun_action_versbase(etat, salle);
	
	case INTERRUPT_ENNEMY:
    Log_info(CAT_FOURMIS, "INTERRUPT RESULT ATTACK\n");
		for (size_t i=0; i<salle->taille_liste; ++i) {
			fourmis_compteur compt = salle->compteurs_fourmis[i];
			if (compt.equipe != mem->team_id)
				return commun_action_attaque(compt.equipe);
		}
    Error(CAT_MAIN, "INTERRUPT ENNEMY: NO ENNEMY\n");
    break;

	case AUCUN_INTERRUPT:
    Error(CAT_MAIN, "NO INTERRUPT\n");
    break;
	
	default:
    Error(CAT_MAIN, "UNKNOWN INTERRUPT\n");
	}
  return commun_action_attendre();
}

fourmi_retour fourmi_act(fourmi_etat *etat, const salle *salle) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  switch (mem->type) {
  case ANT_KIND_FOOD:
    return food_action(etat, salle);
  case ANT_KIND_SCOUT:
    return scout_action(etat, salle);
  case ANT_KIND_GUARD:
    return guard_action(etat, salle);
  case ANT_KIND_NEW:
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW CHERCHE ACTION\n");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN CHERCHE ACTION\n");
  }
  return commun_action_attendre();
}

void fourmi_postaction(fourmi_retour ret, fourmi_etat *etat,
                       const salle *salle) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  switch (mem->type) {
  case ANT_KIND_FOOD:
    break;
  case ANT_KIND_GUARD:
    break;
  case ANT_KIND_SCOUT:
    scout_postaction(etat, salle);
    break;
  case ANT_KIND_NEW:
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW CHERCHE POSTACTION\n");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN CHERCHE POSTACTION\n");
  }
  commun_postaction(etat, salle);
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;

  fourmi_feedback(etat, salle);
  fourmi_pp(CAT_OTHER, LOG_DEBUG, etat);
  
  enum fourminterrupt_e inter = interrupt(etat, salle);
  fourmi_retour ret;
  if ((inter != AUCUN_INTERRUPT) && (!(mem->prevent_prehemption)))
    ret = fourmi_interrupt(etat, salle, inter);
  else
	ret = fourmi_act(etat, salle);
  fourmi_postaction(ret, etat, salle);
  
  
  mem->ret = ret;
  retour_pp(CAT_OTHER, LOG_DEBUG, ret);


  return ret;
}

uint32_t fourmi_id(fourmi_etat* etat) {
    memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
    return mem->fourmis_id;
}