#pragma once

#include <stdint.h>

#include "../log.h"

typedef struct fourmi_stats_s {
    uint8_t max_food;
    uint8_t max_vie;
    uint8_t max_eau;
    uint8_t max_degats;
} fourmi_stats_t;

void fourmi_stats_copy(fourmi_stats_t* dst, fourmi_stats_t src);

void fourmi_stats_init(fourmi_stats_t* st);

void fourmi_stats_body_pp(logcat_t cat, loglevel_t level, fourmi_stats_t* stats);