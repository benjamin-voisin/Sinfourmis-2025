#pragma once

#include "../../sinfourmis.h"
#include "log.h"

uint8_t random_dir(fourmi_etat *etat, const salle *salle);

uint8_t random_other_dir(fourmi_etat *etat, const salle *salle);

void retour_pp_body(logcat_t cat, loglevel_t level, fourmi_retour ret);

void retour_pp(logcat_t cat, loglevel_t level, fourmi_retour ret);