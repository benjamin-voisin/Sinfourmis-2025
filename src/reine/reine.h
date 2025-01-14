#pragma once

#include "stdint.h"

#include "../fourmis/utils/stats.h"

typedef struct reine_s {
    uint64_t tick;
    stats_t stats;
} reine_t;

reine_t* reine_alloc();

void reine_free(reine_t* reine);