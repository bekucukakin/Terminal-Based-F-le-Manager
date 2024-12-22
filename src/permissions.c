#include <stdio.h>
#include <sys/stat.h>
#include "permissions.h"
#include "logger.h"

void change_permissions(const char *path, int permissions) {
    if (chmod(path, permissions) == 0) {
        printf("Permissions changed successfully: %s\n", path);
        log_operation("chmod", "Success");
    } else {
        perror("Failed to change permissions");
        log_operation("chmod", "Failed");
    }
}
