#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_operations.h"
#include "logger.h"

void create_file_or_folder(const char *path) {
    if (creat(path, 0644) != -1) {
        printf("File created successfully: %s\n", path);
        log_operation("create", "Success");
    } else {
        perror("Failed to create file");
        log_operation("create", "Failed");
    }
}

void delete_file_or_folder(const char *path) {
    if (unlink(path) == 0) {
        printf("File deleted successfully: %s\n", path);
        log_operation("delete", "Success");
    } else {
        perror("Failed to delete file");
        log_operation("delete", "Failed");
    }
}

void copy_file(const char *src, const char *dest) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("Failed to open source file");
        log_operation("copy", "Failed");
        return;
    }

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Failed to open destination file");
        close(src_fd);
        log_operation("copy", "Failed");
        return;
    }

    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes);
    }

    close(src_fd);
    close(dest_fd);
    printf("File copied successfully.\n");
    log_operation("copy", "Success");
}
void move_file(const char *src, const char *dest) {
    if (rename(src, dest) == 0) {
        printf("File moved successfully: %s -> %s\n", src, dest);
        log_operation("move", "Success");
    } else {
        perror("Failed to move file");
        log_operation("move", "Failed");
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
