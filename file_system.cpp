#include "file_system.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

namespace fs = std::filesystem;

bool create_directory(const std::string& dir_path) {
    try {
        return fs::create_directories(dir_path);
    } catch (...) {
        return false;
    }
}

bool write_file(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path);
    if (!file.is_open()) return false;
    file << content;
    file.close();
    return true;
}

std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) return "Failed to open file\n";

    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

std::string list_files(const std::string& dir_path) {
    std::ostringstream ss;
    try {
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            ss << entry.path().filename().string() << "\n";
        }
    } catch (...) {
        return "Failed to list files\n";
    }
    return ss.str();
}
#include <filesystem>

bool move_file_or_dir(const std::string& source, const std::string& destination) {
    try {
        std::filesystem::rename(source, destination);
        return true;
    } catch (...) {
        return false;
    }
}
