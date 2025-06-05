#include "filemanager.h"
#include <string> // Cần cho string, to_string
#include <filesystem> // Cần cho filesystem::exists, create_directories

using namespace std;
// Định nghĩa biến tĩnh DATA_DIR
const string FileManager::DATA_DIR = "DanhSachLop";

bool FileManager::createDirectory() {
    try {
        if(!filesystem::exists(DATA_DIR)) {
            return filesystem::create_directories(DATA_DIR);
        }
        return true;
    } catch(...) {
        // Cân nhắc log lỗi ở đây
        return false;
    }
}

string FileManager::getFilePath(int maLop) {
    return DATA_DIR + "/" + to_string(maLop) + ".txt";
}

bool FileManager::fileExists(int maLop) {
    return filesystem::exists(getFilePath(maLop));
} 