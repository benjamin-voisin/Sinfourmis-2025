#include "main.h"
#include "../sinfourmis.h"
#include "commun.h"
#include "scout.h"
#include "food.h"
#include "guard.h"
#include "utils/b_constants.h"
#include "utils/utils.h"
#include "utils/interrupts.h"
#include "utils/log.h"

void new_pp(FILE *f, fourmi_etat *etat) {
  fprintf(f, "NEW_ANT {\n");
  commun_body_pp(f, etat);
  fprintf(f, "}\n");
}

void fourmi_pp(FILE *f, fourmi_etat *etat) {
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  switch (mem->type) {
  case ANT_KIND_FOOD:
    food_pp(f, etat);
    break;
  case ANT_KIND_SCOUT:
    scout_pp(f, etat);
    break;
  case ANT_KIND_COMMON:
    commun_pp(f, etat);
    break;
  case ANT_KIND_GUARD:
    guard_pp(f, etat);
    break;
  case ANT_KIND_NEW:
    new_pp(f, etat);
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
      Log_warning(CAT_MAIN, etat->result >= 0, "Feedback Fail RamasseNourriture");
      break;
    case COMMENCE_CONSTRUCTION:
      commun_feedback_commence_construction(etat, salle);
      break;
    case TERMINE_CONSTRUCTION:
      commun_feedback_termine_construction(etat, salle);
      break;
    case ATTAQUE:
      Log_warning(CAT_MAIN, etat->result > 0, "Feedback Fail Attaque");
      break;
    case ATTAQUE_TUNNEL:
      commun_feedback_attaque_tunnel(etat, salle);
      break;
    case FOURMI_PASSE:
      break;
    default:
      Error(CAT_MAIN, "FOURMIS ACTION UNKNOWN");
    }
    break;
  case ANT_KIND_NEW:
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW NO FEEDBACK");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN NO FEEDBACK");
  }
}

fourmi_retour fourmi_interrupt(fourmi_etat *etat, const salle *salle, enum fourminterrupt_e inter) {
	memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
	switch (inter)
	{
	case INTERRUPT_WATER:
	case INTERRUPT_LIFE:
		return commun_action_versbase(etat, salle);
	
	case INTERRUPT_ENNEMY:
		for (size_t i=0; i<salle->taille_liste; ++i) {
			fourmis_compteur compt = salle->compteurs_fourmis[i];
			if (compt.equipe != mem->team_id)
				return commun_action_attaque(compt.equipe);
		}
    Error(CAT_MAIN, "INTERRUPT ENNEMY: NO ENNEMY");
    break;

	case AUCUN_INTERRUPT:
    Error(CAT_MAIN, "NO INTERRUPT");
    break;
	
	default:
    Error(CAT_MAIN, "UNKNOWN INTERRUPT");
	}
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
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW CHERCHE ACTION");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN CHERCHE ACTION");
  }
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
    Error(CAT_MAIN, "FOURMIS TYPE ANT_KIND_NEW CHERCHE POSTACTION");
    break;
  default:
    Error(CAT_MAIN, "FOURMIS TYPE UNKNOWN CHERCHE POSTACTION");
  }
  commun_postaction(etat, salle);
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
  fourmi_feedback(etat, salle);
  fourmi_pp(stdout, etat);
  enum fourminterrupt_e inter = interrupt(etat, salle);
  fourmi_retour ret;
  if (inter != AUCUN_INTERRUPT)
    ret = fourmi_interrupt(etat, salle, inter);
  else
	ret = fourmi_act(etat, salle);
  fourmi_postaction(ret, etat, salle);
  
  memoire_commun_t *mem = (memoire_commun_t *)etat->memoire;
  mem->ret = ret;
  retour_pp(stdout, ret);
  return ret;
}
