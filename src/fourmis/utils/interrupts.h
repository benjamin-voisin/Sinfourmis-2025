#pragma once

#include "../../sinfourmis.h"

enum fourminterrupt_e {
  AUCUN_INTERRUPT,
  INTERRUPT_WATER,
  INTERRUPT_LIFE,
  INTERRUPT_ENNEMY
}; 

enum fourminterrupt_e interrupt(fourmi_etat *etat, const salle *salle);