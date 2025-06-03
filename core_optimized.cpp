#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <chrono>
using namespace std;
using namespace std::filesystem;

// Cấu trúc sinh viên
struct Sinhkhoa{
    int maSV;
    string hoTen;
    string khoa;
    Sinhkhoa(int ma = 0, const string& ten = "", const string& k = "") 
        : maSV(ma), hoTen(ten), khoa(k) {}
};

// Cấu trúc điểm danh
struct DiemDanh{
    string ngay;
    unordered_map<int, int> trangThai;
    
    DiemDanh(const string& date = "") : ngay(date) {}
};

// Cấu trúc lớp
struct Lop{
    int maLop;
    string tenLop;
    unordered_map<int, Sinhkhoa> danhSachSV; // Key: maSV, Value: Sinhkhoa
    vector<DiemDanh> danhSachDiemDanh;
    
    Lop(int ma = 0, const string& ten = "") : maLop(ma), tenLop(ten) {}
};

// Kiểm tra ngày tháng hợp lệ
class DateValidator {
public:
    static bool isValidDate(const string& date) {
        if(date.length() != 10) return false;
        if(date[2] != '/' || date[5] != '/') return false;
        
        // Kiểm tra các ký tự là số
        for(int i = 0; i < 10; i++){
            if(i == 2 || i == 5) continue;
            if(!isdigit(date[i])) return false;
        }
        
        // Kiểm tra ngày tháng hợp lệ
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        
        if(month < 1 || month > 12) return false;
        if(day < 1 || day > 31) return false;
        if(year < 1900 || year > 2100) return false;
        
        // Kiểm tra số ngày trong tháng
        vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Kiểm tra năm nhuận
        if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            daysInMonth[1] = 29;
        }
        
        return day <= daysInMonth[month - 1];
    }
    
    // Lấy ngày hiện tại
    static string getCurrentDate() {
        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);
        auto tm = *localtime(&time_t);
        
        stringstream ss;
        ss << setfill('0') << setw(2) << tm.tm_mday << "/"
           << setfill('0') << setw(2) << (tm.tm_mon + 1) << "/"
           << (tm.tm_year + 1900);
        return ss.str();
    }
};

// Quản lý file
class FileManager {
private:
    static const string DATA_DIR;
public:
    // Tạo thư mục
    static bool createDirectory() {
        try {
            if(!exists(DATA_DIR)) {
                return create_directories(DATA_DIR);
            }
            return true;
        } catch(...) {
            return false;
        }
    }
    
    // Lấy đường dẫn file
    static string getFilePath(int maLop) {
        return DATA_DIR + "/" + to_string(maLop) + ".txt";
    }

    // Kiểm tra file có tồn tại
    static bool fileExists(int maLop) {
        return exists(getFilePath(maLop));
    }
};
const string FileManager::DATA_DIR = "DanhSachLop";

// Tính toán thống kê
class ThongKe {
public:
    // Cấu trúc thống kê lớp
    struct ThongKeLop {
        int tongSinhkhoa;
        int tongBuoiDiemDanh;
        double tiLeDiemDanhTrungBinh;
        unordered_map<int, int> soLanVang;
    };
    
    // Tính toán thống kê
    static ThongKeLop tinhThongKe(const Lop& lop) {
        ThongKeLop tk;
        tk.tongSinhkhoa = lop.danhSachSV.size();
        tk.tongBuoiDiemDanh = lop.danhSachDiemDanh.size();
        
        if(tk.tongBuoiDiemDanh == 0 || tk.tongSinhkhoa == 0) {
            tk.tiLeDiemDanhTrungBinh = 0;
            return tk;
        }
        
        int tongDiemDanh = 0;
        for(const auto& sv : lop.danhSachSV) {
            tk.soLanVang[sv.first] = 0;
        }
        
        for(const auto& diemDanh : lop.danhSachDiemDanh) {
            for(const auto& sv : lop.danhSachSV) {
                auto it = diemDanh.trangThai.find(sv.first);
                if(it != diemDanh.trangThai.end()) {
                    if(it->second == 1) {
                        tongDiemDanh++;
                    } else {
                        tk.soLanVang[sv.first]++;
                    }
                }
            }
        }
        
        tk.tiLeDiemDanhTrungBinh = (double)tongDiemDanh / (tk.tongSinhkhoa * tk.tongBuoiDiemDanh) * 100;
        return tk;
    }
};

// Quản lý lớp
class QuanLyLop{
private:
    unordered_map<int, Lop> danhSachLop; // Key: maLop, Value: Lop
    
public:
    // Thêm lớp với kiểm tra trùng lặp
    bool themLop(int maLop, const string& tenLop) {
        if(FileManager::fileExists(maLop)) {
            cout << "Lớp với mã " << maLop << " đã tồn tại!" << endl;
            return false;
        }
        
        if(tenLop.empty()) {
            cout << "Tên lớp không được để trống!" << endl;
            return false;
        }
        
        danhSachLop[maLop] = Lop(maLop, tenLop);
        cout << "Thêm lớp thành công!" << endl;
        return true;
    }

    // Thêm sinh viên với kiểm tra trùng lặp
    bool themSinhkhoa(int maLop, const Sinhkhoa& sv) {
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại hoặc chưa đọc file!" << endl;
            return false;
        }
        
        if(lopIt->second.danhSachSV.find(sv.maSV) != lopIt->second.danhSachSV.end()) {
            cout << "Sinh viên với mã " << sv.maSV << " đã tồn tại trong lớp!" << endl;
            return false;
        }
        
        lopIt->second.danhSachSV[sv.maSV] = sv;
        return true;
    }

    // Thêm điểm danh với validation
    bool themDiemDanh(const string& ngay, int maLop, int maSV, int trangThai) {
        if(!DateValidator::isValidDate(ngay)) {
            cout << "Định dạng ngày không hợp lệ! Vui lòng nhập theo định dạng dd/mm/yyyy" << endl;
            return false;
        }
        
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return false;
        }
        
        if(lopIt->second.danhSachSV.find(maSV) == lopIt->second.danhSachSV.end()) {
            cout << "Sinh viên với mã " << maSV << " không tồn tại trong lớp!" << endl;
            return false;
        }
        
        // Tìm ngày điểm danh
        for(auto& diemDanh : lopIt->second.danhSachDiemDanh) {
            if(diemDanh.ngay == ngay) {
                diemDanh.trangThai[maSV] = trangThai;
                return true;
            }
        }
        
        // Tạo mới nếu chưa có
        DiemDanh diemDanhMoi(ngay);
        diemDanhMoi.trangThai[maSV] = trangThai;
        lopIt->second.danhSachDiemDanh.push_back(diemDanhMoi);
        return true;
    }

    // Điểm danh
    bool diemDanh(const string& ngay, int maLop) {
        if(!DateValidator::isValidDate(ngay)) {
            cout << "Định dạng ngày không hợp lệ!" << endl;
            return false;
        }
        
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return false;
        }
        
        if(lopIt->second.danhSachSV.empty()) {
            cout << "Lớp chưa có sinh viên nào!" << endl;
            return false;
        }
        
        // Kiểm tra điểm danh đã tồn tại
        for(auto& diemDanh : lopIt->second.danhSachDiemDanh) {
            if(diemDanh.ngay == ngay) {
                char choice;
                cout << "Điểm danh đã tồn tại. Bạn có muốn điểm danh lại không? (y/n): ";
                cin >> choice;
                if(choice == 'y' || choice == 'Y') {
                    return diemDanhLai(diemDanh, lopIt->second);
                }
                return false;
            }
        }
        
        return diemDanhMoi(ngay, lopIt->second);
    }
    
private:
    bool diemDanhMoi(const string& ngay, Lop& lop) {
        DiemDanh diemDanhMoi(ngay);
        cout << "\nĐIỂM DANH LỚP " << lop.tenLop << " - NGÀY " << ngay << endl;
        cout << string(50, '=') << endl;
        
        int count = 1;
        for(const auto& svPair : lop.danhSachSV) {
            const auto& sv = svPair.second;
            int trangThai;
            do {
                cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
                cin >> trangThai;
                if(trangThai != 0 && trangThai != 1) {
                    cout << "Vui lòng nhập 0 hoặc 1!" << endl;
                }
            } while(trangThai != 0 && trangThai != 1);
            
            diemDanhMoi.trangThai[sv.maSV] = trangThai;
            count++;
        }
        
        lop.danhSachDiemDanh.push_back(diemDanhMoi);
        cout << "Điểm danh thành công!" << endl;
        return true;
    }
    
    bool diemDanhLai(DiemDanh& diemDanh, const Lop& lop) {
        cout << "\nĐIỂM DANH LẠI - NGÀY " << diemDanh.ngay << endl;
        cout << string(50, '=') << endl;
        
        int count = 1;
        for(const auto& svPair : lop.danhSachSV) {
            const auto& sv = svPair.second;
            int trangThai;
            
            do {
                cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
                cin >> trangThai;
                if(trangThai != 0 && trangThai != 1) {
                    cout << "Vui lòng nhập 0 hoặc 1!" << endl;
                }
            } while(trangThai != 0 && trangThai != 1);
            
            diemDanh.trangThai[sv.maSV] = trangThai;
            count++;
        }
        
        cout << "Điểm danh lại thành công!" << endl;
        return true;
    }

public:
    // Sửa điểm danh
    bool suaDiemDanh(const string& ngay, int maLop, int maSV, int trangThai) {
        if(!DateValidator::isValidDate(ngay)) {
            cout << "Định dạng ngày không hợp lệ!" << endl;
            return false;
        }
        
        if(trangThai != 0 && trangThai != 1) {
            cout << "Trạng thái không hợp lệ!" << endl;
            return false;
        }
        
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return false;
        }
        
        if(lopIt->second.danhSachSV.find(maSV) == lopIt->second.danhSachSV.end()) {
            cout << "Sinh viên không tồn tại trong lớp!" << endl;
            return false;
        }
        
        for(auto& diemDanh : lopIt->second.danhSachDiemDanh) {
            if(diemDanh.ngay == ngay) {
                diemDanh.trangThai[maSV] = trangThai;
                cout << "Sửa điểm danh thành công!" << endl;
                return true;
            }
        }
        
        cout << "Không tìm thấy điểm danh cho ngày " << ngay << endl;
        return false;
    }

    // Xuất điểm danh
    void xuatDiemDanh(const string& ngay, int maLop) {
        if(!DateValidator::isValidDate(ngay)) {
            cout << "Định dạng ngày không hợp lệ!" << endl;
            return;
        }
        
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return;
        }
        
        for(const auto& diemDanh : lopIt->second.danhSachDiemDanh) {
            if(diemDanh.ngay == ngay) {
                cout << "\nBẢNG ĐIỂM DANH" << endl;
                cout << "Lớp: " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << endl;
                cout << "Ngày: " << ngay << endl;
                cout << string(80, '=') << endl;
                cout << left << setw(15) << "Mã SV" << setw(30) << "Họ và tên" 
                     << setw(20) << "Khoa" << "Trạng thái" << endl;
                cout << string(80, '-') << endl;
                
                int coMat = 0, vang = 0;
                for(const auto& svPair : lopIt->second.danhSachSV) {
                    const auto& sv = svPair.second;
                    auto it = diemDanh.trangThai.find(sv.maSV);
                    string trangThai = (it != diemDanh.trangThai.end() && it->second == 1) ? "Có mặt" : "Vắng";
                    
                    if(it != diemDanh.trangThai.end() && it->second == 1) coMat++;
                    else vang++;
                    
                    cout << left << setw(15) << sv.maSV << setw(30) << sv.hoTen 
                         << setw(20) << sv.khoa << trangThai << endl;
                }
                
                cout << string(80, '-') << endl;
                cout << "Tổng kết: " << coMat << " có mặt, " << vang << " vắng" << endl;
                cout << "Tỷ lệ điểm danh: " << fixed << setprecision(1)<< (double)coMat / (coMat + vang) * 100 << "%" << endl;
                return;
            }
        }
        
        cout << "Không tìm thấy điểm danh cho ngày " << ngay << endl;
    }

    // Xem lịch sử điểm danh
    void xemLichSu(int maLop) {
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return;
        }
        
        cout << "\nLỊCH SỬ ĐIỂM DANH LỚP " << lopIt->second.tenLop << endl;
        cout << string(60, '=') << endl;
        
        for(const auto& diemDanh : lopIt->second.danhSachDiemDanh) {
            cout << "Ngày: " << diemDanh.ngay;
            int vang =0;int coMat =0;
            for(const auto& svPair : lopIt->second.danhSachSV) {
                auto it = diemDanh.trangThai.find(svPair.first);
                if(it != diemDanh.trangThai.end() && it->second == 1) coMat++;
                else vang++;
            }
            cout << "Tổng số "<< coMat+vang<<" sinh viên - Có mặt: " << coMat << " Vắng: " << vang << endl;
        }
    }
    

    // Xem thống kê lớp
    void xemThongKe(int maLop) {
        auto lopIt = danhSachLop.find(maLop);
        if(lopIt == danhSachLop.end()) {
            cout << "Lớp không tồn tại!" << endl;
            return;
        }
        
        auto tk = ThongKe::tinhThongKe(lopIt->second);
        
        cout << "\nTHỐNG KÊ LỚP " << lopIt->second.tenLop << endl;
        cout << string(60, '=') << endl;
        cout << "Tổng số sinh viên: " << tk.tongSinhkhoa << endl;
        cout << "Tổng số buổi học: " << tk.tongBuoiDiemDanh << endl;
        cout << "Tỷ lệ điểm danh trung bình: " << fixed << setprecision(1) 
             << tk.tiLeDiemDanhTrungBinh << "%" << endl;
        
        if(!tk.soLanVang.empty()) {
            cout << "\nDANH SÁCH SINH VIÊN VẮNG NHIỀU:" << endl;
            cout << string(60, '-') << endl;
            
            vector<pair<int, int>> danhSachVang;
            for(const auto& sv : tk.soLanVang) {
                if(sv.second > 0) {
                    danhSachVang.push_back({sv.second, sv.first});
                }
            }
            
            sort(danhSachVang.rbegin(), danhSachVang.rend());
            
            for(const auto& item : danhSachVang) {
                auto svIt = lopIt->second.danhSachSV.find(item.second);
                if(svIt != lopIt->second.danhSachSV.end()) {
                    cout << "• " << svIt->second.hoTen << " (MSV: " << item.second 
                         << ") - Vắng " << item.first << " buổi" << endl;
                }
            }
        }
    }

    // Lưu file
    bool saveDiemDanh() {
        if(!FileManager::createDirectory()) {
            cout << "Không thể tạo thư mục lưu trữ!" << endl;
            return false;
        }
        
        for(const auto& lopPair : danhSachLop) {
            const auto& lop = lopPair.second;
            ofstream file(FileManager::getFilePath(lop.maLop));
            
            if(!file.is_open()) {
                cout << "Không thể lưu file cho lớp " << lop.maLop << endl;
                continue;
            }
            
            // Lưu thông tin lớp
            file << lop.maLop << " - " << lop.tenLop << "\n";
            
            // Lưu danh sách sinh viên
            for(const auto& svPair : lop.danhSachSV) {
                const auto& sv = svPair.second;
                file << sv.maSV << "|" << sv.hoTen << "|" << sv.khoa << "\n";
            }
            
            // Lưu điểm danh
            for(const auto& diemDanh : lop.danhSachDiemDanh) {
                file << diemDanh.ngay << "\n";
                for(const auto& svPair : lop.danhSachSV) {
                    auto it = diemDanh.trangThai.find(svPair.first);
                    int trangThai = (it != diemDanh.trangThai.end()) ? it->second : 0;
                    file << svPair.first << "-" << trangThai << "\n";
                }
            }
            file.close();
        }
        
        cout << "Lưu dữ liệu thành công!" << endl;
        return true;
    }

    // Đọc file
    bool docFile(int maLop) {
        if(!FileManager::fileExists(maLop)) {
            cout << "File không tồn tại cho lớp " << maLop << endl;
            return false;
        }
        
        ifstream file(FileManager::getFilePath(maLop));
        if(!file.is_open()) {
            cout << "Không thể mở file!" << endl;
            return false;
        }
        
        // Xóa lớp cũ nếu có
        danhSachLop.erase(maLop);
        
        string line;
        if(!getline(file, line)) {
            cout << "File rỗng hoặc bị lỗi!" << endl;
            return false;
        }
        
        // Đọc thông tin lớp
        size_t pos = line.find(" - ");
        if(pos == string::npos) {
            cout << "Format file không đúng!" << endl;
            return false;
        }
        
        string tenLop = line.substr(pos + 3);
        themLop(maLop, tenLop);
        
        // Đọc sinh viên
        while(getline(file, line) && line.find('/') == string::npos) {
            if(line.empty()) continue;
            
            istringstream iss(line);
            string maSVStr, hoTen, khoa;
            
            if(getline(iss, maSVStr, '|') && 
               getline(iss, hoTen, '|') && 
               getline(iss, khoa)) {
                try {
                    int maSV = stoi(maSVStr);
                    themSinhkhoa(maLop, Sinhkhoa(maSV, hoTen, khoa));
                } catch(...) {
                    cout << "Bỏ qua dòng không hợp lệ: " << line << endl;
                }
            }
        }
        
        // Đọc điểm danh
        do {
            if(line.empty() || line.find('/') == string::npos) continue;
            
            string ngay = line;
            while(getline(file, line) && !line.empty()) {
                size_t dashPos = line.find('-');
                if(dashPos != string::npos) {
                    try {
                        int maSV = stoi(line.substr(0, dashPos));
                        int trangThai = stoi(line.substr(dashPos + 1));
                        themDiemDanh(ngay, maLop, maSV, trangThai);
                    } catch(...) {
                        cout << "Bỏ qua dòng điểm danh không hợp lệ: " << line << endl;
                    }
                }
            }
        } while(getline(file, line));
        
        file.close();
        cout << "Đọc dữ liệu thành công!" << endl;
        return true;
    }
    
    bool lopTonTai(int maLop) const {
        if(!FileManager::fileExists(maLop)) {
            cout << "File không tồn tại cho lớp " << maLop << endl;
            return false;
        }
        return true;
    }
    
    int getSoLuongSinhkhoa(int maLop) const {
        auto it = danhSachLop.find(maLop);
        return (it != danhSachLop.end()) ? it->second.danhSachSV.size() : 0;
    }
    
    vector<int> getDanhSachMaLop() const {
        vector<int> result;
        for(const auto& pair : danhSachLop) {
            result.push_back(pair.first);
        }
        return result;
    }
};

// Enhanced Menu class
class Menu{
private:
    QuanLyLop ql;
    
    int readInt(const string& prompt = "") {
        int value;
        cout << prompt;
        while(!(cin >> value)) {
            cout << "Vui lòng nhập số nguyên hợp lệ: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        return value;
    }
    
    void pauseScreen() {
        cout << "\nNhấn Enter để tiếp tục...";
        cin.get();
    }
    
public:
    void menu(){
        cout << "🎓 === HỆ THỐNG QUẢN LÝ ĐIỂM DANH === 🎓" << endl;
        cout << "Ngày hiện tại: " << DateValidator::getCurrentDate() << endl;
        
        while(true){
            cout << "\n" << string(40, '=') << endl;
            cout << "MENU CHÍNH" << endl;
            cout << string(40, '=') << endl;
            cout << "1. Thêm lớp học" << endl;
            cout << "2. Thêm sinh viên" << endl;
            cout << "3. Điểm danh" << endl;
            cout << "4. Sửa điểm danh" << endl;
            cout << "5. Xuất bảng điểm danh" << endl;
            cout << "6. Lịch sử điểm danh" << endl;
            cout << "7. Thống kê lớp" << endl;
            cout << "8. Đọc dữ liệu từ file" << endl;
            cout << "9. Thoát chương trình" << endl;
            cout << string(40, '=') << endl;
            
            int choice = readInt("Nhập lựa chọn (1-9): ");
            cin.ignore();
            
            switch(choice){
                case 1: handleThemLop(); break;
                case 2: handleThemSinhkhoa(); break;
                case 3: handleDiemDanh(); break;
                case 4: handleSuaDiemDanh(); break;
                case 5: handleXuatDiemDanh(); break;
                case 6: handleTimLichSu(); break;
                case 7: handleXemThongKe(); break;
                case 8: handleDocFile(); break;
                case 9: 
                    cout << "Cảm ơn bạn đã sử dụng hệ thống! Tạm biệt!" << endl;
                    return;
                default:
                    cout << "Lựa chọn không hợp lệ!" << endl;
                    break;
            }
            
            pauseScreen();
        }
    }
    
private:
    void handleThemLop() {
        cout << "\nTHÊM LỚP HỌC" << endl;
        int maLop = readInt("Nhập mã lớp: ");
        cout << "Nhập tên lớp: ";
        string tenLop;
        cin.ignore();
        getline(cin, tenLop);
        
        if(ql.themLop(maLop, tenLop)) {
            ql.saveDiemDanh();
        }
    }
    
    void handleThemSinhkhoa() {
        cout << "\n👥 === THÊM SINH VIÊN ===" << endl;
        int maLop = readInt("Nhập mã lớp: ");
        
        if(!ql.lopTonTai(maLop)) {
            cout << "Lớp không tồn tại!" << endl;
            return;
        }
        int n = readInt("Nhập số lượng sinh viên cần thêm: ");
        if(n <= 0) {
            cout << "Số lượng không hợp lệ!" << endl;
            return;
        }
        
        int added = 0;
        for(int i = 0; i < n; i++) {
            cout << "\n--- Sinh viên thứ " << (i+1) << " ---" << endl;
            int maSV = readInt("Mã sinh viên: ");
            cin.ignore();
            
            cout << "Họ và tên: ";
            string hoTen;
            getline(cin, hoTen);
            
            cout << "Khoa: ";
            string khoa;
            getline(cin, khoa);
            
            if(hoTen.empty() || khoa.empty()) {
                cout << "Thông tin không được để trống!" << endl;
                i--;
                continue;
            }
            
            if(ql.themSinhkhoa(maLop, Sinhkhoa(maSV, hoTen, khoa))) {
                added++;
            }
        }
        
        cout << "Đã thêm " << added << "/" << n << " sinh viên thành công!" << endl;
        if(added > 0) ql.saveDiemDanh();
    }
    
    void handleDiemDanh() {
        cout << "\n=== ĐIỂM DANH ===" << endl;
        string ngayHienTai = DateValidator::getCurrentDate();
        cout << "Ngày hiện tại: " << ngayHienTai << endl;
        cout << "Nhập ngày điểm danh (dd/mm/yyyy, Enter để dùng ngày hiện tại): ";
        
        string ngay;
        getline(cin, ngay);
        if(ngay.empty()) {
            ngay = ngayHienTai;
        }
        
        int maLop = readInt("Nhập mã lớp: ");
        
        if(ql.diemDanh(ngay, maLop)) {
            ql.saveDiemDanh();
        }
    }
    
    void handleSuaDiemDanh() {
        cout << "\nSỬA ĐIỂM DANH" << endl;
        cout << "Nhập ngày (dd/mm/yyyy): ";
        string ngay;
        getline(cin, ngay);
        
        int maLop = readInt("Nhập mã lớp: ");
        int maSV = readInt("Nhập mã sinh viên: ");
        int trangThai = readInt("Nhập trạng thái (1=Có mặt/0=Vắng): ");
        
        if(ql.suaDiemDanh(ngay, maLop, maSV, trangThai)) {
            ql.saveDiemDanh();
        }
    }
    
    void handleXuatDiemDanh() {
        cout << "\nXUẤT BẢNG ĐIỂM DANH" << endl;
        cout << "Nhập ngày (dd/mm/yyyy): ";
        string ngay;
        getline(cin, ngay);
        
        int maLop = readInt("Nhập mã lớp: ");
        ql.xuatDiemDanh(ngay, maLop);
    }
    
    void handleTimLichSu() {
        cout << "\nLỊCH SỬ ĐIỂM DANH" << endl;
        int maLop = readInt("Nhập mã lớp: ");
        int maSV = readInt("Nhập mã sinh viên: ");
        
        ql.xemLichSu(maLop);
    }
    
    void handleXemThongKe() {
        cout << "\nTHỐNG KÊ LỚP" << endl;
        int maLop = readInt("Nhập mã lớp: ");
        ql.xemThongKe(maLop);
    }
    
    void handleDocFile() {
        cout << "\nĐỌC DỮ LIỆU TỪ FILE" << endl;
        int maLop = readInt("Nhập mã lớp: ");
        ql.docFile(maLop);
    }
};

int main(){
    try {
        Menu m;
        m.menu();
    } catch(const exception& e) {
        cout << "Lỗi nghiêm trọng: " << e.what() << endl;
        return 1;
    }
    return 0;
} 