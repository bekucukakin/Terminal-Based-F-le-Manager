#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "directory_ops.h"
#include "logger.h"
#include "string.h"
void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Failed to open directory");
        log_operation("list", "Failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat file_stat;
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            printf("%-20s %10ld bytes\n", entry->d_name, file_stat.st_size);
        }
    }

    closedir(dir);
    log_operation("list", "Success");
}

void search_file(const char *dir_path, const char *file_name) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Failed to open directory");
        log_operation("search", "Failed");
        return;
    }

    struct dirent *entry;
    int found = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, file_name) != NULL) {
            printf("Found: %s/%s\n", dir_path, entry->d_name);
            found = 1;
        }
    }

    closedir(dir);

    if (found) {
        log_operation("search", "Success");
    } else {
        printf("No matching file found in directory: %s\n", dir_path);
        log_operation("search", "Not Found");
    }
}