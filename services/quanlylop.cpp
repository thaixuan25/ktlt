#include "quanlylop.h"
#include <iostream> // Cho cout, cin
#include <fstream>  // Cho ofstream, ifstream
#include <sstream>  // Cho istringstream
#include <iomanip>  // Cho setw, left, fixed, setprecision
#include <limits>   // Cho numeric_limits

using namespace std;


// --- Định nghĩa các phương thức của QuanLyLop ---

// Private helper methods (nếu có, ví dụ từ code gốc)
bool QuanLyLop::diemDanhMoi(const string& ngay, Lop& lop) {
    DiemDanh diemDanhMoiObj(ngay); // Đổi tên để tránh trùng với tham số `ngay` của hàm ngoài
    cout << "\nĐIỂM DANH LỚP " << lop.tenLop << " - NGÀY " << ngay << endl;
    cout << string(50, '=') << endl;
    
    int count = 1;
    for(const auto& svPair : lop.danhSachSV) {
        const auto& sv = svPair.second;
        int trangThai;
        do {
            cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
            cin >> trangThai;
            if(cin.fail() || (trangThai != 0 && trangThai != 1)) {
                cout << "Vui lòng nhập 0 hoặc 1!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm sau khi đọc số
        
        diemDanhMoiObj.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    lop.danhSachDiemDanh.push_back(diemDanhMoiObj);
    cout << "Điểm danh thành công!" << endl;
    return true;
}

bool QuanLyLop::diemDanhLai(DiemDanh& diemDanh, const Lop& lop) {
    cout << "\nĐIỂM DANH LẠI - NGÀY " << diemDanh.ngay << endl;
    cout << string(50, '=') << endl;
    
    int count = 1;
    for(const auto& svPair : lop.danhSachSV) {
        const auto& sv = svPair.second;
        int trangThai;
        
        do {
            cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
            cin >> trangThai;
            if(cin.fail() || (trangThai != 0 && trangThai != 1)) {
                cout << "Vui lòng nhập 0 hoặc 1!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
        
        diemDanh.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    cout << "Điểm danh lại thành công!" << endl;
    return true;
}

// Public methods
bool QuanLyLop::themLop(int maLop, const string& tenLop) {
    if(FileManager::fileExists(maLop)) { // Giả sử file tồn tại nghĩa là lớp đã tồn tại (theo logic cũ)
        cout << "Lớp với mã " << maLop << " đã tồn tại (dựa trên file)! Cân nhắc đọc file trước nếu muốn cập nhật." << endl;
        // Hoặc kiểm tra trong danhSachLop nếu logic là lớp trong bộ nhớ
        // if(danhSachLop.count(maLop)) {
        //     cout << "Lớp với mã " << maLop << " đã tồn tại trong bộ nhớ!" << endl;
        //     return false;
        // }
        return false;
    }
    
    if(tenLop.empty()) {
        cout << "Tên lớp không được để trống!" << endl;
        return false;
    }
    
    danhSachLop[maLop] = Lop(maLop, tenLop);
    cout << "Thêm lớp " << tenLop << " (mã: " << maLop << ") vào bộ nhớ thành công. Hãy lưu file để giữ thay đổi." << endl;
    return true;
}

bool QuanLyLop::themSinhkhoa(int maLop, const Sinhkhoa& sv) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ! Hãy thêm lớp hoặc đọc từ file trước." << endl;
        return false;
    }
    
    if(lopIt->second.danhSachSV.count(sv.maSV)) {
        cout << "Sinh viên với mã " << sv.maSV << " đã tồn tại trong lớp " << maLop << "!" << endl;
        return false;
    }
    
    lopIt->second.danhSachSV[sv.maSV] = sv;
    // cout << "Thêm sinh viên " << sv.hoTen << " vào lớp " << maLop << " thành công." << endl; // Có thể bỏ bớt thông báo này nếu quá nhiều
    return true;
}

bool QuanLyLop::themDiemDanh(const string& ngay, int maLop, int maSV, int trangThai) {
    if(!DateValidator::isValidDate(ngay)) {
        cout << "Định dạng ngày '" << ngay << "' không hợp lệ! Vui lòng nhập theo định dạng dd/mm/yyyy." << endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return false;
    }
    
    if(!lopIt->second.danhSachSV.count(maSV)) {
        cout << "Sinh viên với mã " << maSV << " không tồn tại trong lớp " << maLop << "!" << endl;
        return false;
    }
    
    // Tìm ngày điểm danh trong danhSachDiemDanh của lớp
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            ddNgay.trangThai[maSV] = trangThai;
            // cout << "Cập nhật điểm danh cho SV " << maSV << " ngày " << ngay << " thành công." << endl;
            return true;
        }
    }
    
    // Nếu không tìm thấy ngày, tạo mới bản ghi điểm danh cho ngày đó
    DiemDanh diemDanhMoiNgay(ngay);
    diemDanhMoiNgay.trangThai[maSV] = trangThai;
    lopIt->second.danhSachDiemDanh.push_back(diemDanhMoiNgay);
    // cout << "Thêm điểm danh mới cho SV " << maSV << " ngày " << ngay << " thành công." << endl;
    return true;
}

bool QuanLyLop::diemDanh(const string& ngay, int maLop) {
    if(!DateValidator::isValidDate(ngay)) {
        cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return false;
    }
    
    if(lopIt->second.danhSachSV.empty()) {
        cout << "Lớp " << maLop << " chưa có sinh viên nào!" << endl;
        return false;
    }
    
    // Kiểm tra điểm danh đã tồn tại cho ngày này
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            char choice;
            cout << "Điểm danh cho lớp " << lopIt->second.tenLop << " ngày " << ngay << " đã tồn tại. Bạn có muốn điểm danh lại không? (y/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
            if(choice == 'y' || choice == 'Y') {
                return diemDanhLai(ddNgay, lopIt->second);
            }
            return false; // Người dùng không muốn điểm danh lại
        }
    }
    
    // Nếu chưa có, thực hiện điểm danh mới
    return diemDanhMoi(ngay, lopIt->second);
}

bool QuanLyLop::suaDiemDanh(const string& ngay, int maLop, int maSV, int trangThai) {
    if(!DateValidator::isValidDate(ngay)) {
        cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << endl;
        return false;
    }
    
    if(trangThai != 0 && trangThai != 1) {
        cout << "Trạng thái điểm danh không hợp lệ (phải là 0 hoặc 1)!" << endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return false;
    }
    
    if(!lopIt->second.danhSachSV.count(maSV)) {
        cout << "Sinh viên " << maSV << " không tồn tại trong lớp " << maLop << "!" << endl;
        return false;
    }
    
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            if(ddNgay.trangThai.count(maSV)) {
                ddNgay.trangThai[maSV] = trangThai;
                cout << "Sửa điểm danh cho sinh viên " << maSV << " ngày " << ngay << " thành công!" << endl;
                return true;
            } else {
                cout << "Không tìm thấy bản ghi điểm danh cho sinh viên " << maSV << " vào ngày " << ngay << " để sửa." << endl;
                return false;
            }
        }
    }
    
    cout << "Không tìm thấy điểm danh cho ngày " << ngay << " của lớp " << maLop << " để sửa." << endl;
    return false;
}

void QuanLyLop::xuatDiemDanh(const string& ngay, int maLop) {
    if(!DateValidator::isValidDate(ngay)) {
        cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << endl;
        return;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return;
    }
    
    for(const auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            cout << "\nBẢNG ĐIỂM DANH" << endl;
            cout << "Lớp: " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << endl;
            cout << "Ngày: " << ngay << endl;
            cout << string(80, '=') << endl;
            cout << left << setw(10) << "Ma SV" << setw(30) << "Ho va ten" 
                      << setw(20) << "Khoa" << setw(15) << "Trang thai" << endl;
            cout << string(80, '-') << endl;
            
            int coMat = 0, vang = 0;
            if (lopIt->second.danhSachSV.empty()) {
                cout << "Lớp chưa có sinh viên nào." << endl;
            } else {
                for(const auto& svPair : lopIt->second.danhSachSV) {
                    const auto& sv = svPair.second;
                    auto itTrangThai = ddNgay.trangThai.find(sv.maSV);
                    string trangThaiOutput = "Vắng (chưa điểm danh)"; // Mặc định

                    if(itTrangThai != ddNgay.trangThai.end()) {
                        trangThaiOutput = (itTrangThai->second == 1) ? "Co mat" : "Vang";
                        if(itTrangThai->second == 1) coMat++; else vang++;
                    } else {
                        vang++; // Nếu không có trong bản ghi điểm danh, coi là vắng
                    }
                    
                    cout << left << setw(10) << sv.maSV << setw(30) << sv.hoTen 
                              << setw(20) << sv.khoa << setw(15) << trangThaiOutput << endl;
                }
            }
            
            cout << string(80, '-') << endl;
            int tongSV = lopIt->second.danhSachSV.size();
            cout << "Tổng số sinh viên: " << tongSV << endl;
            cout << "Có mặt: " << coMat << ", Vắng: " << vang << endl;
            if (tongSV > 0) {
                cout << "Tỷ lệ có mặt: " << fixed << setprecision(1) 
                          << (static_cast<double>(coMat) / tongSV) * 100.0 << "%" << endl;
            } else {
                 cout << "Tỷ lệ có mặt: 0.0%" << endl;
            }
            return;
        }
    }
    
    cout << "Không tìm thấy bản ghi điểm danh cho lớp " << maLop << " vào ngày " << ngay << "." << endl;
}

void QuanLyLop::xemLichSu(int maLop) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return;
    }
    
    cout << "\nLỊCH SỬ ĐIỂM DANH LỚP " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << endl;
    cout << string(60, '=') << endl;
    
    if (lopIt->second.danhSachDiemDanh.empty()) {
        cout << "Chưa có lịch sử điểm danh nào cho lớp này." << endl;
        return;
    }

    for(const auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        cout << "Ngày: " << ddNgay.ngay;
        int vangCount = 0; int coMatCount = 0;
        int soSvDaDiemDanh = 0;

        // Đếm dựa trên danh sách sinh viên hiện tại của lớp
        for(const auto& svPair : lopIt->second.danhSachSV) {
            auto itTrangThai = ddNgay.trangThai.find(svPair.first);
            if(itTrangThai != ddNgay.trangThai.end()) {
                soSvDaDiemDanh++;
                if(itTrangThai->second == 1) coMatCount++;
                else vangCount++;
            }
        }
        if (soSvDaDiemDanh > 0) {
             cout << " - Số SV được điểm danh: " << soSvDaDiemDanh << " (Có mặt: " << coMatCount << ", Vắng: " << vangCount << ")" << endl;
        } else if (!lopIt->second.danhSachSV.empty() && !ddNgay.trangThai.empty()) {
            cout << " - Bản ghi điểm danh ngày này có " << ddNgay.trangThai.size() << " mục, nhưng không khớp SV hiện tại." << endl;
        } else {
            cout << " - Không có dữ liệu điểm danh cụ thể cho sinh viên vào ngày này hoặc lớp không có SV." << endl;
        }
    }
}

void QuanLyLop::xemThongKe(int maLop) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lop " << maLop << " khong ton tai trong bo nho!" << endl;
        return;
    }
    
    ThongKe::ThongKeLop tk = ThongKe::tinhThongKe(lopIt->second);
    
    cout << "\nTHONG KE LOP " << lopIt->second.tenLop << " (Ma: " << maLop << ")" << endl;
    cout << string(60, '=') << endl;
    cout << "Tong so sinh vien: " << tk.tongSinhkhoa << endl;
    cout << "Tong so buoi da diem danh: " << tk.tongBuoiDiemDanh << endl;
    cout << "Ty le diem danh trung binh toan lop: " << fixed << setprecision(2) 
              << tk.tiLeDiemDanhTrungBinh << "%" << endl;
    
    if (tk.tongSinhkhoa > 0 && tk.tongBuoiDiemDanh > 0) {
        // In duong vien tren
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '=') << endl;
        
        // In tieu de
        cout << left << setw(25) << "Ho va ten" << left << setw(12) << "Ma SV";
        for(const auto& ngay : lopIt->second.danhSachDiemDanh) {
            cout << left << setw(12) << ngay.ngay;
        }
        cout << left << setw(10) << "Tong" << endl;
        
        // In duong vien giua
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '-') << endl;
        
        // In du lieu tung sinh vien
        for(const auto& svPair : tk.tongketSinhvien) {
            cout << left << setw(25) << svPair.second.hoTen << left << setw(12) << svPair.first;
            for(const auto& ngay : tk.tongketSinhvien[svPair.first].trangThai) {
                string trangThaiStr = (ngay.second == 1) ? "Co mat" : "Vang";
                cout << left << setw(12) << trangThaiStr;
            }
            cout << left << setw(10) << tk.tongketSinhvien[svPair.first].tongSoLuotCoMat << endl;
        }
        
        // In duong vien duoi
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '=') << endl;
    }
    else if (tk.tongSinhkhoa == 0) {
        cout << "Lop chua co sinh vien." << endl;
    } else {
        cout << "Lop chua co buoi diem danh nao." << endl;
    }
}

bool QuanLyLop::saveDiemDanh() {
    if(!FileManager::createDirectory()) { // Đảm bảo thư mục tồn tại
        cout << "Lỗi: Không thể tạo hoặc truy cập thư mục lưu trữ '" << FileManager::getFilePath(0).substr(0, FileManager::getFilePath(0).find_last_of("/\\")) << "'!" << endl; // Cố gắng lấy tên thư mục
        return false;
    }
    
    if (danhSachLop.empty()) {
        cout << "Không có dữ liệu lớp nào trong bộ nhớ để lưu." << endl;
        return true; // Không có gì để lưu, coi như thành công
    }

    int countSaved = 0;
    for(const auto& lopPair : danhSachLop) {
        const auto& lop = lopPair.second;
        string filePath = FileManager::getFilePath(lop.maLop);
        ofstream file(filePath);
        
        if(!file.is_open()) {
            cout << "Lỗi: Không thể mở file để lưu cho lớp " << lop.maLop << " tại đường dẫn: " << filePath << endl;
            continue; // Bỏ qua lớp này, tiếp tục với các lớp khác
        }
        
        // Lưu thông tin lớp: MaLop - TenLop
        file << lop.maLop << " - " << lop.tenLop << "\n";
        
        // Lưu danh sách sinh viên: MaSV|HoTen|Khoa
        for(const auto& svPair : lop.danhSachSV) {
            const auto& sv = svPair.second;
            file << sv.maSV << "|" << sv.hoTen << "|" << sv.khoa << "\n";
        }
        
        // Lưu điểm danh
        // Dấu phân cách giữa danh sách SV và các ngày điểm danh
        file << "---DIEMDANH---\n"; 

        for(const auto& ddNgay : lop.danhSachDiemDanh) {
            file << ddNgay.ngay << "\n"; // Ngày điểm danh
            // Lưu trạng thái của từng sinh viên trong ngày đó
            for(const auto& trangThaiPair : ddNgay.trangThai) {
                // Chỉ lưu những sinh viên có trong bản ghi điểm danh của ngày đó
                file << trangThaiPair.first << "-" << trangThaiPair.second << "\n"; // MaSV-TrangThai
            }
            file << "--ENDNGAY--\n"; // Dấu kết thúc cho một ngày
        }
        file.close();
        countSaved++;
    }
    
    if (countSaved > 0) {
        cout << "Đã lưu dữ liệu của " << countSaved << " lớp thành công!" << endl;
    } else if (!danhSachLop.empty()) {
        cout << "Không lưu được dữ liệu của lớp nào (có thể do lỗi mở file)." << endl;
        return false;
    }
    return true;
}

bool QuanLyLop::docFile(int maLopDoc) {
    string filePath = FileManager::getFilePath(maLopDoc);
    if(!FileManager::fileExists(maLopDoc)) { // Sử dụng maLopDoc thay vì chỉ fileExists()
        cout << "File không tồn tại cho lớp " << maLopDoc << " tại: " << filePath << endl;
        return false;
    }
    
    ifstream file(filePath);
    if(!file.is_open()) {
        cout << "Lỗi: Không thể mở file cho lớp " << maLopDoc << " tại: " << filePath << endl;
        return false;
    }
    
    // Xóa dữ liệu lớp cũ (nếu có) trong bộ nhớ trước khi đọc từ file
    // Hoặc, bạn có thể quyết định gộp dữ liệu nếu muốn
    danhSachLop.erase(maLopDoc); 
    
    string line;
    
    // 1. Đọc thông tin lớp (dòng đầu tiên)
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
    
    string maLopStr = line.substr(0, posDash);
    string tenLop;
    try {
        // int maLopFromFile = stoi(maLopStr); // Không cần vì đã có maLopDoc
        tenLop = line.substr(posDash + 3);
    } catch (const exception& e) {
        cout << "Lỗi khi xử lý thông tin lớp từ dòng: '" << line << "'. Chi tiết: " << e.what() << endl;
        file.close();
        return false;
    }

    // Tạo lớp mới trong danhSachLop
    Lop lopMoi(maLopDoc, tenLop);
    // danhSachLop[maLopDoc] = lopMoi; // Sẽ thêm vào cuối sau khi đọc hết

    // 2. Đọc danh sách sinh viên
    while(getline(file, line) && line != "---DIEMDANH---") {
        if(line.empty()) continue;
        
        istringstream iss(line);
        string sv_maStr, sv_hoTen, sv_khoa;
        
        if(getline(iss, sv_maStr, '|') && 
           getline(iss, sv_hoTen, '|') && 
           getline(iss, sv_khoa)) {
            try {
                int sv_ma = stoi(sv_maStr);
                lopMoi.danhSachSV[sv_ma] = Sinhkhoa(sv_ma, sv_hoTen, sv_khoa);
            } catch(const exception& e) {
                cout << "Cảnh báo: Bỏ qua dòng sinh viên không hợp lệ: '" << line << "'. Lỗi: " << e.what() << endl;
            }
        } else {
            cout << "Cảnh báo: Bỏ qua dòng sinh viên có định dạng không đúng: '" << line << "'" << endl;
        }
    }

    // 3. Đọc điểm danh (sau dòng "---DIEMDANH---")
    string ngayCurrent;
    DiemDanh diemDanhNgayObj; // Khai báo ngoài vòng lặp ngày

    while(getline(file, line)) {
        if(line.empty()) continue;

        if(line == "--ENDNGAY--") { // Kết thúc một ngày
            if(!ngayCurrent.empty() && !diemDanhNgayObj.trangThai.empty()) {
                lopMoi.danhSachDiemDanh.push_back(diemDanhNgayObj);
            }
            ngayCurrent = ""; // Reset cho ngày tiếp theo
            // diemDanhNgayObj.trangThai.clear(); // Không cần clear nếu tạo mới khi gặp ngày mới
            continue;
        }

        // Nếu dòng này không phải là ENDNGAY và ngayCurrent đang rỗng, thì đây là dòng ngày mới
        if(ngayCurrent.empty()) { 
            if(DateValidator::isValidDate(line)){
                ngayCurrent = line;
                diemDanhNgayObj = DiemDanh(ngayCurrent); // Tạo đối tượng DiemDanh mới cho ngày này
            } else {
                 cout << "Cảnh báo: Bỏ qua dòng ngày không hợp lệ: '" << line << "'" << endl;
                 // Bỏ qua các dòng trạng thái cho đến khi gặp ENDNGAY hoặc ngày hợp lệ mới
                 while(getline(file, line) && line != "--ENDNGAY--") {} 
                 if (file.eof() && line != "--ENDNGAY--") break; // Nếu hết file mà chưa gặp ENDNGAY
                 ngayCurrent = ""; // Reset
            }
            continue;
        }
        
        // Nếu đã có ngayCurrent, dòng này là dòng trạng thái MaSV-TrangThai
        if (!ngayCurrent.empty()) {
            size_t dashPosDD = line.find('-');
            if(dashPosDD != string::npos) {
                try {
                    int maSV_dd = stoi(line.substr(0, dashPosDD));
                    int trangThai_dd = stoi(line.substr(dashPosDD + 1));
                    
                    // Chỉ thêm nếu sinh viên tồn tại trong danh sách sinh viên của lớp
                    if(lopMoi.danhSachSV.count(maSV_dd) && (trangThai_dd == 0 || trangThai_dd == 1)) {
                         diemDanhNgayObj.trangThai[maSV_dd] = trangThai_dd;
                    } else if (!lopMoi.danhSachSV.count(maSV_dd)){
                        cout << "Cảnh báo: Bỏ qua điểm danh cho SV " << maSV_dd << " (không có trong lớp) ngày " << ngayCurrent << endl;
                    } else {
                        cout << "Cảnh báo: Bỏ qua điểm danh cho SV " << maSV_dd << " (trạng thái " << trangThai_dd << " không hợp lệ) ngày " << ngayCurrent << endl;
                    }
                } catch(const exception& e) {
                    cout << "Cảnh báo: Bỏ qua dòng điểm danh không hợp lệ: '" << line << "'. Lỗi: " << e.what() << endl;
                }
            } else {
                 cout << "Cảnh báo: Bỏ qua dòng điểm danh có định dạng sai: '" << line << "' cho ngày " << ngayCurrent << endl;
            }
        }
    }
    // Xử lý ngày cuối cùng nếu file không kết thúc bằng --ENDNGAY-- (mặc dù nên có)
    if(!ngayCurrent.empty() && !diemDanhNgayObj.trangThai.empty()) {
        lopMoi.danhSachDiemDanh.push_back(diemDanhNgayObj);
    }

    danhSachLop[maLopDoc] = lopMoi; // Thêm lớp đã đọc vào map
    file.close();
    return true;
}

bool QuanLyLop::lopTonTai(int maLop) const {
    // Kiểm tra cả trong bộ nhớ và trên file theo logic cũ
    // return danhSachLop.count(maLop) || FileManager::fileExists(maLop);
    // Hoặc chỉ kiểm tra trong bộ nhớ nếu đó là ý định
    return danhSachLop.count(maLop);
    // Hoặc, nếu lopTonTai chỉ có nghĩa là file vật lý tồn tại:
    // return FileManager::fileExists(maLop);
    // --> Theo logic sử dụng trong menu, có vẻ là file vật lý:
    // if(!ql.lopTonTai(maLop)) { cout << "Lớp không tồn tại!" << endl; return; }
    // Tuy nhiên, hàm themLop lại kiểm tra fileExists.
    // Để nhất quán, có lẽ nên là kiểm tra sự tồn tại trong danhSachLop (dữ liệu đã nạp)
    // Nếu muốn kiểm tra file thì gọi FileManager::fileExists trực tiếp.
    // Quyết định: lopTonTai nghĩa là đã được nạp vào danhSachLop.
}

int QuanLyLop::getSoLuongSinhkhoa(int maLop) const {
    auto it = danhSachLop.find(maLop);
    return (it != danhSachLop.end()) ? it->second.danhSachSV.size() : 0;
}

vector<int> QuanLyLop::getDanhSachMaLop() const {
    vector<int> result;
    result.reserve(danhSachLop.size()); // Cấp phát trước để tránh realloc nhiều lần
    for(const auto& pair : danhSachLop) {
        result.push_back(pair.first);
    }
    return result;
} 