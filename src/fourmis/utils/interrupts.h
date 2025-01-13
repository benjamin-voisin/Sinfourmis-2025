#pragma once

#include "../../sinfourmis.h"

typedef enum fourminterrupt_e {
  AUCUN_INTERRUPT,
  INTERRUPT_WATER,
  INTERRUPT_LIFE,
  INTERRUPT_ENNEMY,
  NB_INTERRUPTS
} fourminterrupt_t;

typedef uint8_t interruptstats_t[NB_INTERRUPTS];

fourminterrupt_t interrupt(fourmi_etat *etat, const salle *salle);