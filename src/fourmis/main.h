#ifndef ANT_MAIN_H
#define ANT_MAIN_H

#include "../sinfourmis.h"
#include "../utils/log.h"

void fourmi_pp(logcat_t cat, loglevel_t level, fourmi_etat* etat);
uint8_t	fourmi_kind(fourmi_etat* etat);
uint32_t fourmi_id(fourmi_etat* etat);

#endif
