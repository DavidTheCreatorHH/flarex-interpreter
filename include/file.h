#ifndef FILE_H
#define FILE_H

#include <string>

extern void init_path(const std::string &file_path);

extern int get_file_str(std::string file_path, std::string &content);
extern int write_file(const std::string &file_name, const std::string &content, std::string file_path = "");
extern int create_file(std::string file_name, std::string file_path = "");
extern int remove_file(const std::string &file_name, std::string o_ext);

#endif
