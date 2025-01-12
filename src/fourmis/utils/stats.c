#include "stats.h"

void stats_copy(stats_t* dst, stats_t src) {
    dst->max_degats = src.max_degats;
    dst->max_eau = src.max_eau;
    dst->max_food = src.max_food;
    dst->max_vie = src.max_vie;
}