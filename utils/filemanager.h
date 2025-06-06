#ifndef FILEMANAGER_H
#define FILEMANAGER_H

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
    static bool createDirectory();
    static string getFilePath(int maLop);
    static bool fileExists(int maLop);
    static bool saveLop(const Lop& lop);
    static bool readLop(int maLop, Lop& outLop);
};
#endif 