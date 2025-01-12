#pragma once

#include <stdbool.h>

#define ANSI_RESET "\033[0m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_RED_BOLD "\033[1;31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_MAGENTA_BOLD "\033[1;35m"
#define ANSI_COLOR_CYAN "\033[36m"
#define ANSI_COLOR_LIGHT_GRAY "\033[37m"

#if __has_attribute(format) || defined(__GNUC__)
#define PRINTF_FORMAT(fmt, args) __attribute__((__format__(printf, fmt, args)))
#else
#define PRINTF_FORMAT(fmt, args)
#endif

typedef enum loglevel_e {    
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ASSERT,
    LOG_ERROR,
    LOG_FATAL
} loglevel_t;

typedef enum logcat_e {
    CAT_OTHER,
    CAT_MAIN,
    CAT_PILE,
    CAT_FOURMIS,
    CAT_TODO,
    CAT_NOBLOAT,
} logcat_t;


#define Log_debug(cat, ...) Log(cat, LOG_DEBUG, __VA_ARGS__)
#define Log_info(cat, ...) Log(cat, LOG_INFO, __VA_ARGS__)
#define Log_warning(cat, ...) Log(cat, LOG_WARNING, __VA_ARGS__)

void Log_set_level(loglevel_t level);

PRINTF_FORMAT(3, 4) void Log(logcat_t cat, loglevel_t level, const char *format, ...);

PRINTF_FORMAT(3, 4) void Assert(logcat_t cat, bool cond, const char *format, ...);

PRINTF_FORMAT(2, 3) void Error(logcat_t cat, const char *format, ...);

PRINTF_FORMAT(2, 3) void Fatal(logcat_t cat, const char *format, ...);