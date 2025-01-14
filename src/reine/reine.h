#pragma once

#include "stdint.h"

#include "../fourmis/utils/stats.h"

typedef struct reine_s {
    uint64_t tick;
    stats_t stats;
} reine_t;

reine_t* reine_alloc();

void reine_free(reine_t* reine);

void reine_pp(logcat_t cat, loglevel_t level, reine_t* reine);