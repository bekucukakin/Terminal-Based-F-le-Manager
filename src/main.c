#include <stdio.h>
#include <string.h>
#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"
#include "logger.h"

void show_usage() {
    printf("Usage:\n\n");

    // List files in the directory
    printf("  slist <directory_path>\n");
    printf("    List files in the specified directory.\n");
    printf("    Example: slist /home/user/documents\n\n");

    // Copy file to the specified path
    printf("  scopy <source_file> <destination_path>\n");
    printf("    Copy the specified source file to the destination path.\n");
    printf("    Example: scopy /home/user/file.txt /home/user/docs/\n\n");

    // Remove file or directory
    printf("  sremove <file_or_directory_path>\n");
    printf("    Remove the specified file or directory.\n");
    printf("    Example: sremove /home/user/file.txt\n");
    printf("    Example: sremove /home/user/docs\n\n");

    // Move file to the specified path
    printf("  smove <source_file> <destination_path>\n");
    printf("    Move the source file to the destination path.\n");
    printf("    Example: smove /home/user/file.txt /home/user/docs/\n\n");

    // Change file or directory permissions
    printf("  schmod <file_or_directory_path> <permissions>\n");
    printf("    Change the permissions of the specified file or directory.\n");
    printf("    Example: schmod /home/user/file.txt 755\n\n");

    printf("For creating file in detail, you need to run our program using 'file_manager' command \n");
    printf("For more information, use the 'help' command.\n");
}


void print_menu() {
    printf("\n=== Terminal Based File Manager ===\n");
    printf("1. List Files (ls)\n");
    printf("2. Create File/Folder (creat/mkdir)\n");
    printf("3. Delete File/Folder (unlink/rmdir)\n");
    printf("4. Copy File (copy)\n");
    printf("5. Move File (move)\n");
    printf("6. View File Content (view)\n");
    printf("7. Write Content to File (write)\n");
    printf("8. Search File (search)\n");
    printf("9. Change Permissions (chmod)\n");
    printf("10. Exit\n");
}

int main(int argc, char *argv[]) {
 
    if (argc > 1) {
        
        if (strcmp(argv[1], "slist") == 0) {
            if (argc != 3) {
                show_usage();
                return 1;
            }
            list_files(argv[2]); 
        } 
        else if (strcmp(argv[1], "scopy") == 0) {
            if (argc != 4) {
                show_usage();
                return 1;
            }
            copy_file(argv[2], argv[3]); 
        } 
        else if (strcmp(argv[1], "sremove") == 0) {
            if (argc != 3) {
                show_usage();
                return 1;
            }
            delete_file_or_folder(argv[2]); 
        }
        else if (strcmp(argv[1], "smove") == 0) {
            if (argc != 4) {
                show_usage();
                return 1;
            }
        move_file(argv[2],argv[3]); 
        }
          else if (strcmp(argv[1], "schmod") == 0) {
            if (argc != 4) {
                show_usage();
                return 1;
            }
        change_permissions(argv[2],argv[3]); 
        } 
        else if (strcmp(argv[1], "help") == 0) {
            show_usage(); 
        }
        else {
            printf("Unknown command: %s\n", argv[1]);
            show_usage(); 
        }
        return 0;
    }

    
    char command[256];
    char path[256];
    char content[256];
    int choice;

    init_logger("/home/ozangul/Desktop/Terminal-Based-File-Manager/logs/operations.log");

    while (1) {
        print_menu();
        printf("Enter your choice: ");
        
        
        if (scanf("%d", &choice) != 1) {
            
            while(getchar() != '\n');
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

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
                printf("Enter file path to write something: ");
                scanf("%s", path);
                printf("Enter something to write: ");
                scanf("%s", content);
                write_to_file(path, content);
                return 0;
            case 8:
                printf("Enter folder path to search: ");
                scanf("%s", path);
                printf("Enter file name to search: ");
                scanf("%s", command);
                search_file(path, command);
                break;
            case 9:
                printf("Enter file/folder path: ");
                scanf("%s", path);
                printf("Enter new permissions (e.g., 0755): ");
                int permissions;
                scanf("%o", &permissions);
                change_permissions(path, permissions);
                break;
            case 10:
                printf("Exiting...\n");
                close_logger();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
