#include "stats.h"

#include "../../constants.hpp"

void stats_copy(stats_t* dst, stats_t src) {
    dst->max_degats = src.max_degats;
    dst->max_eau = src.max_eau;
    dst->max_food = src.max_food;
    dst->max_vie = src.max_vie;
}

stats_t stats_init() {
    stats_t st;
    st.max_degats = DEFAULT_ATTACK;
    st.max_eau = DEFAULT_MAX_WATER;
    st.max_food = DEFAULT_MAX_FOOD;
    st.max_vie = DEFAULT_MAX_LIFE;
    return st;
}

void stats_body_pp(logcat_t cat, loglevel_t level, stats_t* stats) {
    Log(cat, level, "STATS:\n");
    Log(cat, level, "    max_deg    = %u\n", stats->max_degats);
    Log(cat, level, "    max_eau    = %u\n", stats->max_eau);
    Log(cat, level, "    max_food   = %u\n", stats->max_food);
    Log(cat, level, "    max_vie    = %u\n", stats->max_vie);
}