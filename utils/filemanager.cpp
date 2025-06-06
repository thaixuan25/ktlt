#include "filemanager.h"
#include <fstream>
#include <iostream>
#include <sstream>

// --- Định nghĩa các hằng và phương thức của FileManager ---

// Thư mục lưu trữ dữ liệu
const string FileManager::DATA_DIR = "DanhSachLop/";

bool FileManager::createDirectory() {
    try {
        if (!exists(DATA_DIR)) {
            create_directories(DATA_DIR);
        }
        return true;
    } catch (const filesystem_error& e) {
        cout << "Lỗi filesystem: " << e.what() << endl;
        return false;
    }
}

string FileManager::getFilePath(int maLop) {
    return DATA_DIR + to_string(maLop) + ".txt";
}

bool FileManager::fileExists(int maLop) {
    return exists(getFilePath(maLop));
}

// Logic lưu file được chuyển từ QuanLyLop::saveDiemDanh
bool FileManager::saveLop(const Lop& lop) {
    string filePath = getFilePath(lop.maLop);
    ofstream file(filePath);
    
    if(!file.is_open()) {
        cout << "Lỗi: Không thể mở file để lưu cho lớp " << lop.maLop << " tại đường dẫn: " << filePath << endl;
        return false;
    }
    
    // Ghi thông tin lớp
    file << lop.maLop << " - " << lop.tenLop << "\n";
    
    // Ghi danh sách sinh viên
    for(const auto& svPair : lop.danhSachSV) {
        const auto& sv = svPair.second;
        file << sv.maSV << "|" << sv.hoTen << "|" << sv.khoa << "\n";
    }

    // Dấu phân cách
    file << "---DIEMDANH---\n"; 

    // Ghi lịch sử điểm danh
    for(const auto& ddNgay : lop.danhSachDiemDanh) {
        file << ddNgay.ngay << "\n";
        for(const auto& trangThaiPair : ddNgay.trangThai) {
            file << trangThaiPair.first << "-" << trangThaiPair.second << "\n";
        }
        file << "--ENDNGAY--\n";
    }

    file.close();
    return true;
}

// Logic đọc file được chuyển từ QuanLyLop::docFile
bool FileManager::readLop(int maLop, Lop& outLop) {
    string filePath = getFilePath(maLop);
    ifstream file(filePath);
    
    if(!file.is_open()) {
        // Không cần thông báo lỗi ở đây vì hàm gọi sẽ xử lý
        return false;
    }

    string line;
    if(!getline(file, line)) {
        cout << "Lỗi: File " << filePath << " rỗng hoặc không thể đọc dòng thông tin lớp." << endl;
        file.close();
        return false;
    }
    
    size_t posDash = line.find(" - ");
    if(posDash == string::npos) {
        cout << "Lỗi: Format dòng thông tin lớp không đúng trong file " << filePath << ". Mong đợi: 'MaLop - TenLop'." << endl;
        file.close();
        return false;
    }
    
    string tenLop = line.substr(posDash + 3);
    outLop = Lop(maLop, tenLop); // Khởi tạo đối tượng Lop được truyền vào

    // Đọc danh sách sinh viên
    while(getline(file, line) && line != "---DIEMDANH---") {
        if(line.empty()) continue;
        
        istringstream iss(line);
        string svMaStr, svHoTen, svKhoa;
        
        if(getline(iss, svMaStr, '|') && getline(iss, svHoTen, '|') && getline(iss, svKhoa)) {
            try {
                int svMa = stoi(svMaStr);
                outLop.danhSachSV[svMa] = Sinhvien(svMa, svHoTen, svKhoa);
            } catch(const exception& e) {
                cout << "Cảnh báo: Bỏ qua dòng sinh viên không hợp lệ: '" << line << "'. Lỗi: " << e.what() << endl;
            }
        } else {
            cout << "Cảnh báo: Bỏ qua dòng sinh viên có định dạng không đúng: '" << line << "'" << endl;
        }
    }

    string ngayCurrent;
    DiemDanh diemDanhNgayObj;

    while(getline(file, line)) {
        if(line.empty()) continue;

        if(line == "--ENDNGAY--") {
            if(!ngayCurrent.empty() && !diemDanhNgayObj.trangThai.empty()) {
                outLop.danhSachDiemDanh.push_back(diemDanhNgayObj);
            }
            ngayCurrent = "";
            continue;
        }

        if(ngayCurrent.empty()) { 
            if(DateValidator::isValidDate(line)){
                ngayCurrent = line;
                diemDanhNgayObj = DiemDanh(ngayCurrent);
            } else {
                 cout << "Cảnh báo: Bỏ qua dòng ngày không hợp lệ: '" << line << "'" << endl;
                 while(getline(file, line) && line != "--ENDNGAY--") {} 
                 if (file.eof() && line != "--ENDNGAY--") break;
                 ngayCurrent = "";
            }
            continue;
        }
        
        if (!ngayCurrent.empty()) {
            size_t dashPosDD = line.find('-');
            if(dashPosDD != string::npos) {
                try {
                    int maSvDd = stoi(line.substr(0, dashPosDD));
                    int trangThaiDd = stoi(line.substr(dashPosDD + 1));
                    
                    if(outLop.danhSachSV.count(maSvDd) && (trangThaiDd == 0 || trangThaiDd == 1)) {
                         diemDanhNgayObj.trangThai[maSvDd] = trangThaiDd;
                    }
                } catch(const exception& e) {
                    cout << "Cảnh báo: Bỏ qua dòng điểm danh không hợp lệ: '" << line << "'. Lỗi: " << e.what() << endl;
                }
            }
        }
    }
    if(!ngayCurrent.empty() && !diemDanhNgayObj.trangThai.empty()) {
        outLop.danhSachDiemDanh.push_back(diemDanhNgayObj);
    }

    file.close();
    return true;
} 