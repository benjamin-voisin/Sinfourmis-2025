#include "fourmi_stats.h"

#include "../../constants.hpp"

void fourmi_stats_copy(fourmi_stats_t* dst, fourmi_stats_t src) {
    dst->max_degats = src.max_degats;
    dst->max_eau = src.max_eau;
    dst->max_food = src.max_food;
    dst->max_vie = src.max_vie;
}

void fourmi_stats_init(fourmi_stats_t st) {
    st.max_degats = DEFAULT_ATTACK;
    st.max_eau = DEFAULT_MAX_WATER;
    st.max_food = DEFAULT_MAX_FOOD;
    st.max_vie = DEFAULT_MAX_LIFE;
}

void fourmi_stats_body_pp(logcat_t cat, loglevel_t level, fourmi_stats_t* stats) {
    Log(cat, level, "FOURMI STATS:\n");
    Log(cat, level, "    max_deg    = %u\n", stats->max_degats);
    Log(cat, level, "    max_eau    = %u\n", stats->max_eau);
    Log(cat, level, "    max_food   = %u\n", stats->max_food);
    Log(cat, level, "    max_vie    = %u\n", stats->max_vie);
}