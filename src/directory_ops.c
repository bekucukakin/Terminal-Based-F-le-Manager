#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "directory_ops.h"
#include "logger.h"
#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif
#include <asm-generic/fcntl.h>
#include <unistd.h>
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
        DIR *dir = opendir(path);
        if (dir == NULL) {
            log_error("delete_file_or_folder", "Failed to open directory: %s", path);
            perror("Failed to open directory");
            log_operation("delete folder", "Failed");
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Skip the "." and ".." entries
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            char entry_path[PATH_MAX];
            snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);

            // Recursively delete files and subdirectories
            delete_file_or_folder(entry_path);
        }

        closedir(dir);

        // Now that the directory is empty, delete it
        if (rmdir(path) == 0) {
            printf("Directory deleted successfully: %s\n", path);
            log_operation("delete folder", "Success");
        } else {
            log_error("delete_file_or_folder", "Failed to delete directory at path: %s", path);
            perror("Failed to delete directory");
            log_operation("delete folder", "Failed");
        }
    } else if (S_ISREG(path_stat.st_mode)) {
        if (unlink(path) == 0) {
            printf("File deleted successfully: %s\n", path);
            log_operation("delete file", "Success");
        } else {
            log_error("delete_file_or_folder", "Failed to delete file at path: %s", path);
            perror("Failed to delete file");
            log_operation("delete file", "Failed");
        }
    } else {
        printf("The path is neither a file nor a directory: %s\n", path);
        log_error("delete_file_or_folder", "Path is neither a file nor a directory: %s", path);
        log_operation("delete", "Failed");
    }
}
void write_to_file(const char *filepath, const char *content) {
    struct stat st;
    
    // Check if the file exists
    if (stat(filepath, &st) == 0 && st.st_size > 0) {
        // File exists and has content
        printf("The file already contains content.\n");
        printf("Do you want to append to the file (A) or overwrite it (O)? ");
        
        char choice;
        scanf(" %c", &choice);  // Read user input

        if (choice == 'A' || choice == 'a') {
            // Append to the file
            int fd = open(filepath, O_WRONLY | O_APPEND, 0644);
            if (fd < 0) {
                log_error("write_to_file", "Failed to open file for appending: %s", filepath);
                perror("Failed to open file for appending");
                return;
            }

            ssize_t bytes_written = write(fd, content, strlen(content));
            if (bytes_written != strlen(content)) {
                log_error("write_to_file", "Error writing to file: %s", filepath);
                perror("Error writing to file");
                close(fd);
                return;
            }

            close(fd);
            printf("Content successfully appended to %s.\n", filepath);
            log_operation("write", "Append success");

        } else if (choice == 'O' || choice == 'o') {
            // Overwrite the file
            int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                log_error("write_to_file", "Failed to open file for writing: %s", filepath);
                perror("Failed to open file for writing");
                return;
            }

            ssize_t bytes_written = write(fd, content, strlen(content));
            if (bytes_written != strlen(content)) {
                log_error("write_to_file", "Error writing to file: %s", filepath);
                perror("Error writing to file");
                close(fd);
                return;
            }

            close(fd);
            printf("Content successfully written to %s.\n", filepath);
            log_operation("write", "Overwrite success");
        } else {
            printf("Invalid choice. No action taken.\n");
        }
    } else {
        // File does not exist or is empty, so just write the content
        int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            log_error("write_to_file", "Failed to open file for writing: %s", filepath);
            perror("Failed to open file for writing");
            return;
        }

        ssize_t bytes_written = write(fd, content, strlen(content));
        if (bytes_written != strlen(content)) {
            log_error("write_to_file", "Error writing to file: %s", filepath);
            perror("Error writing to file");
            close(fd);
            return;
        }

        close(fd);
        printf("Content successfully written to %s.\n", filepath);
        log_operation("write", "Success");
    }
}

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