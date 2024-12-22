#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

void create_file_or_folder(const char *path);
void delete_file_or_folder(const char *path);
void copy_file(const char *src, const char *dest);
void move_file(const char *src, const char *dest);
void view_file(const char *filepath);
void search_file(const char *dir_path, const char *file_name);
#endif
