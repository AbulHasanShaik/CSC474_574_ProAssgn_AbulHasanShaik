#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

// Create a directory
bool create_directory(const std::string& dir_path);

// Write content to a file
bool write_file(const std::string& file_path, const std::string& content);

// Read and return content from a file
std::string read_file(const std::string& file_path);

// List all files in a directory
std::string list_files(const std::string& dir_path);

bool move_file_or_dir(const std::string& source, const std::string& destination);


#endif // FILE_SYSTEM_H
