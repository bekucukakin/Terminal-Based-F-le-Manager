#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

void copy_file(const char *src, const char *dest);
void move_file(const char *src, const char *dest);
void view_file(const char *filepath);
void search_file(const char *dir_path, const char *file_name);
void write_to_file(const char *filepath, const char *content);
#endif
