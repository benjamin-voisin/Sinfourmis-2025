#include "reine.h"

#include "../utils/safe_allocs.h"
#include "../fourmis/utils/stats.h"

reine_t* reine_alloc() {
    reine_t* reine = (reine_t*) Malloc(sizeof(reine_t));
    reine->tick = 0;
    reine->stats = stats_init();
    return reine;
}

void reine_free(reine_t* reine) {
    free(reine);
}

void reine_body_pp(logcat_t cat, loglevel_t level, reine_t* reine) {
    Log(cat, level, "REINE BODY:\n");
    Log(cat, level, "    ticks      = %u\n", reine->tick);
    stats_body_pp(cat, level, &reine->stats);
}

void reine_pp(logcat_t cat, loglevel_t level, reine_t* reine) {
    Log(cat, level, "REINE {\n");
    reine_body_pp(cat, level, reine);
    Log(cat, level, "}\n");
}