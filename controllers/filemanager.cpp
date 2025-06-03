#include "filemanager.h"
#include <string> // Cần cho std::string, to_string
#include <filesystem> // Cần cho std::filesystem::exists, create_directories

// Định nghĩa biến tĩnh DATA_DIR
const std::string FileManager::DATA_DIR = "DanhSachLop";

bool FileManager::createDirectory() {
    try {
        if(!std::filesystem::exists(DATA_DIR)) {
            return std::filesystem::create_directories(DATA_DIR);
        }
        return true;
    } catch(...) {
        // Cân nhắc log lỗi ở đây
        return false;
    }
}

std::string FileManager::getFilePath(int maLop) {
    return DATA_DIR + "/" + std::to_string(maLop) + ".txt";
}

bool FileManager::fileExists(int maLop) {
    return std::filesystem::exists(getFilePath(maLop));
} 