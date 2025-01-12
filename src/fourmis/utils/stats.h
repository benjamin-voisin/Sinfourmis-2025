#pragma once

#include <stdint.h>

typedef struct stats_s {
    uint8_t max_food;
    uint8_t max_vie;
    uint8_t max_eau;
    uint8_t max_degats;
} stats_t;

void stats_copy(stats_t* dst, stats_t src);

stats_t stats_init();