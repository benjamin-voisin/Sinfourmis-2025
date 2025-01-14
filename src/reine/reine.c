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