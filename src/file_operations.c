#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>  
#include "file_operations.h"
#include "logger.h"
#include <dirent.h>
#include "directory_ops.h"
#include "string.h"


void copy_file(const char *src, const char *dest) {
    struct stat src_stat;

    
    if (stat(src, &src_stat) != 0) {
        log_error("copy_file", "Source file doesn't exist: %s", src);
        perror("Source file doesn't exist");
        return;
    }

    
    if (!S_ISREG(src_stat.st_mode)) {
        printf("Source is not a valid file: %s\n", src);
        log_error("copy_file", "Source is not a valid file: %s", src);
        return;
    }

    
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        log_error("copy_file", "Failed to open source file: %s", src);
        perror("Failed to open source file");
        return;
    }

    struct stat dest_stat;
    
    
    if (stat(dest, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        char dest_path[512];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, strrchr(src, '/') + 1);

        
        int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0) {
            log_error("copy_file", "Failed to open destination file: %s", dest_path);
            perror("Failed to open destination file");
            close(src_fd);
            return;
        }

        
        char buffer[1024];
        ssize_t bytes;
        while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dest_fd, buffer, bytes) != bytes) {
                log_error("copy_file", "Error writing to destination file: %s", dest_path);
                perror("Error writing to destination file");
                close(src_fd);
                close(dest_fd);
                return;
            }
        }

        close(src_fd);
        close(dest_fd);
        printf("File copied successfully to directory %s.\n", dest_path);
    } else {
        
        int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0) {
            log_error("copy_file", "Failed to open destination file: %s", dest);
            perror("Failed to open destination file");
            close(src_fd);
            return;
        }

        
        char buffer[1024];
        ssize_t bytes;
        while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dest_fd, buffer, bytes) != bytes) {
                log_error("copy_file", "Error writing to destination file: %s", dest);
                perror("Error writing to destination file");
                close(src_fd);
                close(dest_fd);
                return;
            }
        }

        close(src_fd);
        close(dest_fd);
        printf("File copied successfully to %s.\n", dest);
    }
}


void move_file(const char *src, const char *dest) {
    struct stat dest_stat;

    
    if (stat(dest, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        
        
        char dest_path[512];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, strrchr(src, '/') + 1);

        
        if (rename(src, dest_path) == 0) {
            printf("File moved successfully: %s -> %s\n", src, dest_path);
            log_operation("move", "Success");
        } else {
            log_error("move_file", "Failed to move file from %s to %s", src, dest_path);
            perror("Failed to move file");
            log_operation("move", "Failed");
        }
    } else {
        
        if (rename(src, dest) == 0) {
            printf("File moved successfully: %s -> %s\n", src, dest);
            log_operation("move", "Success");
        } else {
            log_error("move_file", "Failed to move file from %s to %s", src, dest);
            perror("Failed to move file");
            log_operation("move", "Failed");
        }
    }
}


void view_file(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        log_error("view_file", "Failed to open file: %s", filepath);
        perror("Failed to open file");
        log_operation("view", "Failed");
        return;
    }

    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        
        if (write(STDOUT_FILENO, buffer, bytes) != bytes) {
            log_error("view_file", "Failed to write data to stdout while viewing file: %s", filepath);
            perror("Error writing data to standard output");
            close(fd);
            log_operation("view", "Failed");
            return;
        }
    }

    if (bytes < 0) {
        log_error("view_file", "Failed to read file: %s", filepath);
        perror("Error reading file");
        close(fd);
        log_operation("view", "Failed");
        return;
    }

    close(fd);
    printf("\nFile viewed successfully: %s\n", filepath);
    log_operation("view", "Success");
}

void search_file(const char *dir_path, const char *file_name) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        log_error("search_file", "Failed to open directory: %s", dir_path);
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
        printf("File(s) matching '%s' found in directory: %s\n", file_name, dir_path);
        log_operation("search", "Success");
    } else {
        printf("No matching file found for '%s' in directory: %s\n", file_name, dir_path);
        log_operation("search", "Not Found");
    }
}
