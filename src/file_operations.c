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

void create_file_or_folder(const char *base_path) {
    char name[100], full_path[200];
    int is_folder;

    
    printf("Do you want to create a folder (1) or a file (0)? ");
    scanf("%d", &is_folder);

    
    printf("Enter the name of the file/folder: ");
    scanf("%s", name);

    
    snprintf(full_path, sizeof(full_path), "%s/%s", base_path, name);

    if (is_folder) {
        
        if (mkdir(full_path, 0755) == 0) {
            printf("Folder created successfully: %s\n", full_path);
            log_operation("create folder", "Success");
        } else {
            perror("Failed to create folder");
            log_operation("create folder", "Failed");
        }
    } else {
        
        if (creat(full_path, 0644) != -1) {
            printf("File created successfully: %s\n", full_path);
            log_operation("create file", "Success");
        } else {
            perror("Failed to create file");
            log_operation("create file", "Failed");
        }
    }
}

void delete_file_or_folder(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
        if (rmdir(path) == 0) {
            printf("Directory deleted successfully: %s\n", path);
            log_operation("delete", "Success");
        } else {
            perror("Failed to delete directory");
            log_operation("delete", "Failed");
        }
    } else if (S_ISREG(path_stat.st_mode)) {
        if (unlink(path) == 0) {
            printf("File deleted successfully: %s\n", path);
            log_operation("delete", "Success");
        } else {
            perror("Failed to delete file");
            log_operation("delete", "Failed");
        }
    } else {
        printf("Path is neither a file nor a directory: %s\n", path);
    }
}

void copy_file(const char *src, const char *dest) {
  
    struct stat src_stat;
    if (stat(src, &src_stat) != 0) {
        perror("Source file doesn't exist");
        return;
    }
    if (!S_ISREG(src_stat.st_mode)) {
        printf("Source is not a valid file.\n");
        return;
    }

    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("Failed to open source file");
        return;
    }

    struct stat dest_stat;
    if (stat(dest, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
       
        char dest_path[512];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, strrchr(src, '/') + 1);

        int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0) {
            perror("Failed to open destination file");
            close(src_fd);
            return;
        }

        char buffer[1024];
        ssize_t bytes;
        while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dest_fd, buffer, bytes) != bytes) {
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
            perror("Failed to open destination file");
            close(src_fd);
            return;
        }

        char buffer[1024];
        ssize_t bytes;
        while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dest_fd, buffer, bytes) != bytes) {
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
            perror("Failed to move file");
            log_operation("move", "Failed");
        }
    } else {
    
        if (rename(src, dest) == 0) {
            printf("File moved successfully: %s -> %s\n", src, dest);
            log_operation("move", "Success");
        } else {
            perror("Failed to move file");
            log_operation("move", "Failed");
        }
    }
}

void view_file(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        log_operation("view", "Failed");
        return;
    }

    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes);
    }

    close(fd);
    log_operation("view", "Success");
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