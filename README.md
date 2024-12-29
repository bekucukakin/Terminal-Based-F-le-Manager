# Terminal-Based-File-Manager

### 1. Introduction

The **Terminal-Based File Manager** is a command-line application designed to allow users to perform common file and directory operations such as listing files, copying, moving, deleting, viewing, and modifying file contents and permissions. The program is designed to operate in a Linux-like environment, offering a user-friendly interface that facilitates interaction through simple commands.

This project aims to simulate the basic functionalities of a file manager within the terminal, enabling efficient file management for both novice and experienced users. By integrating various file operations into a single application, users can perform essential tasks without needing a graphical interface.

### 2. Objectives

The primary objectives of the Terminal-Based File Manager project are:

- To provide a terminal-based interface for managing files and directories.
- To enable file management tasks such as listing, copying, moving, deleting, viewing, and modifying files and directories.
- To offer functionality for changing file and directory permissions.
- To include a logging feature to track operations performed by the user.

### 3. Project Structure

The program is structured to be modular, consisting of the following files:

- **main.c**: This is the main file that drives the program's logic, handles user input, and calls the appropriate functions.
- **file_operations.c**: Contains the implementation of functions related to file operations such as creating, copying, and deleting files.
- **directory_ops.c**: Contains functions to handle directory-specific operations such as listing and searching files in a directory.
- **permissions.c**: Provides functions to modify file and directory permissions.
- **logger.c**: Responsible for logging user operations for audit and debugging purposes.
- **file_operations.h, directory_ops.h, permissions.h, logger.h**: Header files that declare the functions used in the respective source files.

### 4. Functionality

The Terminal-Based File Manager offers the following core functionalities:

1. **Listing Files in a Directory**:
    - Command: `slist <directory_path>`
    - The program lists all the files and directories in the specified directory.
2. **Copying Files**:
    - Command: `scopy <source_file> <destination_path>`
    - Allows users to copy a file from one location to another.
3. **Removing Files or Directories**:
    - Command: `sremove <file_or_directory_path>`
    - Removes the specified file or directory.
4. **Moving Files**:
    - Command: `smove <source_file> <destination_path>`
    - Moves the file from one location to another.
5. **Changing File Permissions**:
    - Command: `schmod <file_or_directory_path> <permissions>`
    - Changes the file or directory permissions to the specified value.
6. **Help**:
    - Command: `help`
    - Displays a list of available commands and their descriptions.
7. **Terminal Menu**:
    - Provides users with a text-based interface where they can select an operation from a list of options.

### 5. Program Flow

The main logic of the program revolves around the user's input. The program starts by checking if the user has provided command-line arguments. Based on these arguments, the program determines which file operation to perform. If no arguments are passed, the program presents the user with a menu of options.

- The program handles invalid inputs by displaying an error message and prompting the user for a valid choice.
- Once a user selects an option, the program requests the necessary input (e.g., file paths, permissions) and performs the corresponding operation.
- After completing an operation, the program either displays the result (e.g., a list of files) or returns to the menu for further interaction.

### 6. Logging

The application integrates a logging feature that records every user operation. This helps to track the activities performed during the session, such as file manipulations and permission changes. The logs are saved to a file, which can later be reviewed for auditing or debugging purposes.

### 7. Error Handling

Error handling is implemented throughout the program to ensure smooth user experience:

- Invalid commands or arguments trigger the display of usage instructions.
- Input validation ensures that the program only proceeds with valid file paths, file names, and permissions.
- The program gracefully handles file or directory operations that may fail due to missing permissions or incorrect paths, and appropriate error messages are displayed.

### 8. Get Started With Terminal-Based-File Manager

## Prerequisites

- A Linux-based operating system (tested on Ubuntu).
- `make` tool installed to build the project.

## Installation

1. **Clone the repository**:

```
git clone https://github.com/bekucukakin/Terminal-Based-File-Manager.git
cd terminal-based-file-manager

```

1. **Build the project**:
The project uses a `Makefile` to simplify the build process. Run the following command to compile and build the program:

```
make
```

1. **Run the program**:
After building the project, you can run the file manager with the following command:

```
./file_manager
```

Alternatively, you can use specific commands directly from the terminal:

```
./file_manager slist <directory_path>
./file_manager scopy <source_file> <destination_path>
./file_manager sremove <file_or_directory_path>
./file_manager smove <source_file> <destination_path>
./file_manager schmod <file_or_directory_path> <permissions>
```

### Logging

This program logs all operations performed by the user to a log file. However, **make sure to modify the log initialization** section in the `main.c` file to match your desired log file path before running the program.

To adjust the log file path, locate the following line in `main.c`:

```
init_logger("/home/ozangul/Desktop/Terminal-Based-File-Manager/logs/operations.log");

```

Update it to the appropriate path for your system.

For example:

```
init_logger("/path/to/your/logs/operations.log");
```

This will ensure that the logs are saved in your desired location.

The Terminal-Based File Manager project successfully provides a set of useful file management tools in a command-line environment. It integrates various essential functionalities such as file operations, permission changes, and logging, ensuring a robust and efficient system for managing files and directories. The program is designed to be intuitive and easy to use, providing a text-based alternative for users who prefer terminal interfaces over graphical file managers.

This project demonstrates fundamental concepts in systems programming, including file handling, permission management, and logging, making it a valuable tool for both educational and practical purposes.



### 9. Conclusion

The Terminal-Based File Manager project successfully provides a set of useful file management tools in a command-line environment. It integrates various essential functionalities such as file operations, permission changes, and logging, ensuring a robust and efficient system for managing files and directories. The program is designed to be intuitive and easy to use, providing a text-based alternative for users who prefer terminal interfaces over graphical file managers.

This project demonstrates fundamental concepts in systems programming, including file handling, permission management, and logging, making it a valuable tool for both educational and practical purposes.
