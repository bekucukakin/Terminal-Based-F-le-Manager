#ifndef DIRECTORY_OPS_H
#define DIRECTORY_OPS_H

void list_files(const char *path);
void create_file_or_folder(const char *path);
void delete_file_or_folder(const char *path);
void write_to_file(const char *filepath, const char *content);
#endif
