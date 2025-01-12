#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#define ANSI_RESET "\033[0m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_RED_BOLD "\033[1;31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_MAGENTA_BOLD "\033[1;35m"
#define ANSI_COLOR_CYAN "\033[36m"
#define ANSI_COLOR_LIGHT_GRAY "\033[37m"

const char* str_of_cat(logcat_t cat) {
    switch (cat) {
    case CAT_MAIN:
        return "M";
    case CAT_PILE:
        return "P";
    default:
        return "O";
    }
}

const char* str_of_level(loglevel_t level) {
    switch (level) {
    case LOG_WARNING:
        return "WARN ";
    case LOG_ASSERT:
        return "ASSER";
    case LOG_ERROR:
        return "ERROR";
    case LOG_FATAL:
        return "FATAL";
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO ";
    default:
        return "OTHER";
    }
}


FILE* stdio_of_level(loglevel_t level) {
    switch (level) {
    case LOG_WARNING:
    case LOG_ASSERT:
    case LOG_ERROR:
    case LOG_FATAL:
        return stderr;
    case LOG_DEBUG:
    case LOG_INFO:
    default:
        return stdout;
    }
}

const char* color_of_level(loglevel_t level) {
    switch (level) {
    case LOG_WARNING:
        return ANSI_COLOR_YELLOW;
    case LOG_ASSERT:
        return ANSI_COLOR_RED;
    case LOG_ERROR:
        return ANSI_COLOR_RED_BOLD;
    case LOG_FATAL:
        return ANSI_COLOR_MAGENTA_BOLD;
    case LOG_DEBUG:
        return ANSI_COLOR_CYAN;
    case LOG_INFO:
        return ANSI_COLOR_GREEN;
    default:
        return "";
    }
}


static loglevel_t current_log_level = LOG_DEBUG;

void log_set_level(loglevel_t level) { current_log_level = level; }

static void log_vimpl(logcat_t cat, loglevel_t level, const char *format, va_list args) {
  time_t now = time(0);
  struct tm *local_time = localtime(&now);
  char time_buffer[16];
  time_buffer[strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S",
                       local_time)] = '\0';

  fprintf(stdio_of_level(level), ANSI_COLOR_LIGHT_GRAY "%s" ANSI_RESET " %s[%s](%s) " ANSI_RESET,
          time_buffer, color_of_level(level), str_of_level(level), str_of_cat(cat));
  vfprintf(stdio_of_level(level), format, args);
}

void Log(logcat_t cat, loglevel_t level, const char *format, ...) {
  if (level >= current_log_level) {
    va_list args;
    va_start(args, format);
    log_vimpl(cat, level, format, args);
    va_end(args);
  }
}

void Assert(logcat_t cat, bool cond, const char *format, ...) {
    if (!(cond)) {
        va_list args;
        va_start(args, format);
        log_vimpl(cat, LOG_ASSERT, format, args);
        va_end(args);
        assert(cond);
    }
}

void Error(logcat_t cat, const char *format, ...) {
  va_list args;
  va_start(args, format);
  log_vimpl(cat, LOG_ERROR, format, args);
  va_end(args);
  exit(1);
}

void Fatal(logcat_t cat, const char *format, ...) {
  va_list args;
  va_start(args, format);
  log_vimpl(cat, LOG_FATAL, format, args);
  va_end(args);
  exit(1);
}