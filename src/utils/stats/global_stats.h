#pragma once

#include "fourmi_stats.h"
#include "reine_stats.h"

typedef struct stats_s {
    fourmi_stats_t fourmi_stats;
    reine_stats_t  reine_stats;    
} stats_t;

void stats_init(stats_t stats);

void stats_body_pp(logcat_t cat, loglevel_t level, stats_t* stats);