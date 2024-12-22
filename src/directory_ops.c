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
        return;
    }

    struct dirent *entry;
    struct stat file_stat;

    // Başlıklar
    printf("%-10s %-10s %-10s %s\n", "Permissions", "Size", "Type", "Name");

    // Her bir dosya/dizin için döngü
    while ((entry = readdir(dir)) != NULL) {
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Dosya hakkında bilgi almak için stat fonksiyonu
        if (stat(full_path, &file_stat) == 0) {
            // İzinler
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

            // Dosya türü
            char file_type[10] = "FILE";  // Varsayılan tür (normal dosya)
            if (S_ISDIR(file_stat.st_mode)) {
                strcpy(file_type, "DIR"); // Dizin
            } else if (S_ISLNK(file_stat.st_mode)) {
                strcpy(file_type, "LINK"); // Symbolic link
            } else if (file_stat.st_mode & S_IXUSR) {
                strcpy(file_type, "EXEC"); // Çalıştırılabilir dosya
            }

            // İzinler, boyut, dosya adı
            printf("%-10s %-10ld %-10s %s\n", perms, file_stat.st_size, file_type, entry->d_name);
        }
    }

    closedir(dir);
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