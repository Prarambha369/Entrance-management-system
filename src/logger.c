#include "../include/common.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>

static const char *log_levels[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

void init_log_system() {
    ensure_dir_exists("data");
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        time_t now = time(NULL);
        char time_str[30];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        fprintf(log_file, "\n\n=== Logging started at %s ===\n", time_str);
        fclose(log_file);
    }
}

void log_message(int level, const char *message, ...) {
    if (level < LOG_DEBUG || level > LOG_ERROR) {
        level = LOG_INFO;
    }
    
    time_t now = time(NULL);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Log to file
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        fprintf(log_file, "[%s] [%s] ", time_str, log_levels[level]);
        
        va_list args;
        va_start(args, message);
        vfprintf(log_file, message, args);
        va_end(args);
        
        fprintf(log_file, "\n");
        fclose(log_file);
    }
    
    // Also log to console for errors and warnings
    if (level >= LOG_WARNING) {
        set_color(level == LOG_ERROR ? COLOR_RED : COLOR_YELLOW);
        printf("[%s] ", log_levels[level]);
        set_color(COLOR_RESET);
        
        va_list args;
        va_start(args, message);
        vprintf(message, args);
        va_end(args);
        
        printf("\n");
    }
}

FILE* safe_open(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (!fp) {
        log_message(LOG_ERROR, "Failed to open file: %s (mode: %s)", filename, mode);
    }
    return fp;
}
