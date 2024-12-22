#include <stdio.h>
#include <time.h>
#include "logger.h"

static FILE *log_file;

void init_logger(const char *log_path) {
    log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
    }
}

void log_operation(const char *operation, const char *status) {
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "[%s] Operation: %s, Status: %s\n", ctime(&now), operation, status);
        fflush(log_file);
    }
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
    }
}
