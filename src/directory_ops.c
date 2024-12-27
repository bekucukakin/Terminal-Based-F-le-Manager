#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "directory_ops.h"
#include "logger.h"

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Error: The directory '%s' could not be opened. Please check if the directory exists and if the path is correct.\n", path);
        log_operation("list_files", "Failed - Directory Open Error");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;

    printf("%-10s %-10s %-10s %s\n", "Permissions", "Size", "Type", "Name");

    while ((entry = readdir(dir)) != NULL) {
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Use lstat to properly identify symbolic links
        if (lstat(full_path, &file_stat) == 0) {
            // File permissions
            char perms[11] = "----------";
            perms[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : (S_ISLNK(file_stat.st_mode) ? 'l' : '-');
            perms[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
            perms[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
            perms[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
            perms[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
            perms[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
            perms[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
            perms[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
            perms[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
            perms[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

            // Determine file type
            char file_type[10] = "FILE";
            if (S_ISDIR(file_stat.st_mode)) {
                strcpy(file_type, "DIR");
            } else if (S_ISLNK(file_stat.st_mode)) {
                strcpy(file_type, "LINK");
            }

            // Print file information
            printf("%-10s %-10ld %-10s %s\n", perms, file_stat.st_size, file_type, entry->d_name);
        } else {
            printf("Error: Could not retrieve information for file '%s'. Please check permissions.\n", entry->d_name);
            log_operation("list_files", "Failed - Stat Error");
        }
    }

    closedir(dir);
    log_operation("list_files", "Success");
}