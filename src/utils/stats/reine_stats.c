#include "reine_stats.h"

#include "../../constants.hpp"

void reine_stats_init(reine_stats_t* st) {
    st->max_stockage = QUEEN_DEFAULT_MAX_STORED_ANT;
    st->max_production = QUEEN_DEFAULT_PRODUCTION_DELAY;
    st->max_vitesse_amelioration = QUEEN_DEFAULT_UPGRADE_DURATION;
}

void reine_stats_body_pp(logcat_t cat, loglevel_t level, reine_stats_t* stats) {
    Log(cat, level, "REINE STATS:\n");
    Log(cat, level, "    max_stock  = %u\n", stats->max_stockage);
    Log(cat, level, "    max_prod   = %u\n", stats->max_production);
    Log(cat, level, "    max_vit_amm= %u\n", stats->max_vitesse_amelioration);
}