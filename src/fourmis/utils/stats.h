#pragma once

#include <stdint.h>

#include "log.h"

typedef struct stats_s {
    uint8_t max_food;
    uint8_t max_vie;
    uint8_t max_eau;
    uint8_t max_degats;
} stats_t;

void stats_copy(stats_t* dst, stats_t src);

stats_t stats_init();

void stats_body_pp(logcat_t cat, loglevel_t level, stats_t* stats);