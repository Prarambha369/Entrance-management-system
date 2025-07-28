#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

// Log levels
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

// Function declarations
void init_log_system();
void log_message(int level, const char *format, ...);

#endif // LOGGER_H
