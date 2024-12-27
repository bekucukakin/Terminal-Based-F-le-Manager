#ifndef LOGGER_H
#define LOGGER_H

void init_logger(const char *log_path);
void log_operation(const char *operation, const char *status);
void log_error(const char *function_name, const char *message, ...);
void close_logger();

#endif
