#pragma once

#include <stdint.h>

#include "../../fourmis/utils/log.h"

typedef struct reine_stats_s {
    uint8_t max_stockage;
    uint8_t max_production;
    uint8_t max_vitesse_amelioration;
} reine_stats_t;

void reine_stats_init(reine_stats_t st);

void reine_stats_body_pp(logcat_t cat, loglevel_t level, reine_stats_t* stats);