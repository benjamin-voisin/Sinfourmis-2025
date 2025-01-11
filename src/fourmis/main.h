#ifndef ANT_MAIN_H
#define ANT_MAIN_H

#include "../sinfourmis.h"
#include <stdio.h>

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle);

void fourmi_pp(FILE* f, fourmi_etat* etat);

#endif
