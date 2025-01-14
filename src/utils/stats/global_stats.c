#include "global_stats.h"

void stats_init(stats_t* stats) {
    fourmi_stats_init(&stats->fourmi_stats);
    reine_stats_init(&stats->reine_stats);
}

void stats_body_pp(logcat_t cat, loglevel_t level, stats_t* stats) {
    fourmi_stats_body_pp(cat, level, &stats->fourmi_stats);
    reine_stats_body_pp(cat, level, &stats->reine_stats);
}