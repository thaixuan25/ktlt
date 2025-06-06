#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <filesystem>
#include "../modules/structure.h"
#include "../utils/datevalidator.h"

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

    // Hàm mới để lưu một đối tượng Lop
    static bool saveLop(const Lop& lop);

    // Hàm mới để đọc dữ liệu một lớp vào một đối tượng Lop
    static bool readLop(int maLop, Lop& outLop);
};
#endif 