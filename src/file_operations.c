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
    if (scanf("%d", &is_folder) != 1 || (is_folder != 0 && is_folder != 1)) {
        log_error("create_file_or_folder", "Invalid input for folder/file selection.");
        printf("Invalid input. Please enter 1 for folder or 0 for file.\n");
        return;
    }

    
    printf("Enter the name of the file/folder: ");
    if (scanf("%s", name) != 1 || strlen(name) == 0) {
        log_error("create_file_or_folder", "Invalid or empty name provided.");
        printf("Invalid name. Please provide a valid name for the file/folder.\n");
        return;
    }

    
    snprintf(full_path, sizeof(full_path), "%s/%s", base_path, name);

    
    if (is_folder) {
        if (mkdir(full_path, 0755) == 0) {
            printf("Folder created successfully: %s\n", full_path);
            log_operation("create folder", "Success");
        } else {
            log_error("create_file_or_folder", "Failed to create folder at path: %s", full_path);
            perror("Failed to create folder");
        }
    } 
    
    else {
        if (creat(full_path, 0644) != -1) {
            printf("File created successfully: %s\n", full_path);
            log_operation("create file", "Success");
        } else {
            log_error("create_file_or_folder", "Failed to create file at path: %s", full_path);
            perror("Failed to create file");
        }
    }
}

void delete_file_or_folder(const char *path) {
    struct stat path_stat;

    
    if (stat(path, &path_stat) != 0) {
        log_error("delete_file_or_folder", "Invalid path: %s", path);
        perror("Invalid path");
        log_operation("delete", "Failed");
        return;
    }

    
    if (S_ISDIR(path_stat.st_mode)) {
        if (rmdir(path) == 0) {
            printf("Directory deleted successfully: %s\n", path);
            log_operation("delete folder", "Success");
        } else {
            log_error("delete_file_or_folder", "Failed to delete directory at path: %s", path);
            perror("Failed to delete directory");
            log_operation("delete folder", "Failed");
        }
    }
    
    else if (S_ISREG(path_stat.st_mode)) {
        if (unlink(path) == 0) {
            printf("File deleted successfully: %s\n", path);
            log_operation("delete file", "Success");
        } else {
            log_error("delete_file_or_folder", "Failed to delete file at path: %s", path);
            perror("Failed to delete file");
            log_operation("delete file", "Failed");
        }
    }
    
    else {
        printf("The path is neither a file nor a directory: %s\n", path);
        log_error("delete_file_or_folder", "Path is neither a file nor a directory: %s", path);
        log_operation("delete", "Failed");
    }
}


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
