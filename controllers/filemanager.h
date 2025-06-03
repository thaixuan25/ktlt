#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

// Quản lý file
class FileManager {
private:
    static const string DATA_DIR;
public:
    // Tạo thư mục
    static bool createDirectory();
    
    // Lấy đường dẫn file
    static string getFilePath(int maLop);

    // Kiểm tra file có tồn tại
    static bool fileExists(int maLop);
};
#endif