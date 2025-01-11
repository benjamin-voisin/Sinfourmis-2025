#pragma once

#include "../../sinfourmis.h"

#include <stdio.h>

uint8_t random_dir(fourmi_etat *etat, const salle *salle);

uint8_t random_other_dir(fourmi_etat *etat, const salle *salle);

void retour_pp_body(FILE* f, fourmi_retour ret);

void retour_pp(FILE* f, fourmi_retour ret);