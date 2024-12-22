#include <stdio.h>
#include <string.h>
#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"
#include "logger.h"

void print_menu() {
    printf("\n=== Terminal Based File Manager ===\n");
    printf("1. List Files (ls)\n");
    printf("2. Create File/Folder (creat/mkdir)\n");
    printf("3. Delete File/Folder (unlink/rmdir)\n");
    printf("4. Copy File (copy)\n");
    printf("5. Move File (move)\n");
    printf("6. View File Content (view)\n");
    printf("7. Search File (search)\n");
    printf("8. Change Permissions (chmod)\n");
    printf("9. Exit\n");
}

int main() {
    char command[256];
    char path[256];
    int choice;

    init_logger("logs/operations.log");

    while (1) {
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter directory path: ");
                scanf("%s", path);
                list_files(path);
                break;
            case 2:
                printf("Enter file/folder path to create: ");
                scanf("%s", path);
                create_file_or_folder(path);
                break;
            case 3:
                printf("Enter file/folder path to delete: ");
                scanf("%s", path);
                delete_file_or_folder(path);
                break;
            case 4:
                printf("Enter source path: ");
                scanf("%s", command);
                printf("Enter destination path: ");
                scanf("%s", path);
                copy_file(command, path);
                break;
            case 5:
                printf("Enter source path: ");
                scanf("%s", command);
                printf("Enter destination path: ");
                scanf("%s", path);
                move_file(command, path);
                break;
            case 6:
                printf("Enter file path to view: ");
                scanf("%s", path);
                view_file(path);
                break;
            case 7:
                printf("Enter folder path to search: ");
                scanf("%s", path);
                printf("Enter file name to search: ");
                scanf("%s", command);
                search_file(path, command);
                break;
            case 8:
                printf("Enter file/folder path: ");
                scanf("%s", path);
                printf("Enter new permissions (e.g., 0755): ");
                int permissions;
                scanf("%o", &permissions);
                change_permissions(path, permissions);
                break;
            case 9:
                printf("Exiting...\n");
                close_logger();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
