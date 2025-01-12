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

const char* str_of_color(logcolor_t col) {
    switch (col) {
    case COLOR_RED:
        return ANSI_COLOR_RED;
    case COLOR_GREEN:
        return ANSI_COLOR_GREEN;
    case COLOR_YELLOW:
        return ANSI_COLOR_YELLOW;
    case COLOR_MAGENTA:
        return ANSI_COLOR_MAGENTA;
    case COLOR_CYAN:
        return ANSI_COLOR_CYAN;
    case LIGHT_GRAY:
        return ANSI_COLOR_LIGHT_GRAY;
    default:
        return "";
    }
}

const char* str_of_cat(logcat_t cat) {
    switch (cat) {
    case CAT_MAIN:
        return "M";
    case CAT_PILE:
        return "P";
    case CAT_FOURMIS:
        return "F";
    case CAT_TODO:
        return "T";
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


static logcolor_t current_log_color = LIGHT_GRAY;
void log_set_color(logcolor_t col) { current_log_color = col; }
void log_reset_color() { current_log_color = LIGHT_GRAY; }

static loglevel_t current_log_level = LOG_INFO;
void log_set_level(loglevel_t level) { current_log_level = level; }
void log_reset_level() { current_log_level = LOG_INFO; }


static void log_vimpl(logcat_t cat, loglevel_t level, const char *format, va_list args) {
  time_t now = time(0);
  struct tm *local_time = localtime(&now);
  char time_buffer[16];
  time_buffer[strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S",
                       local_time)] = '\0';

  if (cat != CAT_NOBLOAT) {
    fprintf(stdio_of_level(level), ANSI_COLOR_LIGHT_GRAY "%s" ANSI_RESET " %s[%s](%s) " ANSI_RESET,
          time_buffer, color_of_level(level), str_of_level(level), str_of_cat(cat));
  }
  fprintf(stdio_of_level(level), "%s", str_of_color(current_log_color));
  vfprintf(stdio_of_level(level), format, args);
  fprintf(stdio_of_level(level), ANSI_RESET);
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
        #ifndef RELEASE
        assert(cond);
        #endif
    }
}

void Error(logcat_t cat, const char *format, ...) {
  va_list args;
  va_start(args, format);
  log_vimpl(cat, LOG_ERROR, format, args);
  va_end(args);
  #ifndef RELEASE
  exit(1);
  #endif
}

void Fatal(logcat_t cat, const char *format, ...) {
  va_list args;
  va_start(args, format);
  log_vimpl(cat, LOG_FATAL, format, args);
  va_end(args);
  exit(1);
}