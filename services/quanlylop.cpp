#include "quanlylop.h"
#include <iostream>
#include <iomanip> 

using namespace std;


// --- Định nghĩa các phương thức của QuanLyLop ---

// Điểm danh mới
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
                cin.ignore();
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        cin.ignore();
        
        diemDanhMoiObj.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    lop.danhSachDiemDanh.push_back(diemDanhMoiObj);
    cout << "Điểm danh thành công!" << endl;
    return true;
}

// Điểm danh lại
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
                cin.ignore();
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        cin.ignore();
        
        diemDanh.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    cout << "Điểm danh lại thành công!" << endl;
    return true;
}

// Thêm lớp
bool QuanLyLop::themLop(int maLop, const string& tenLop) {
    if(FileManager::fileExists(maLop)) {
        cout << "Lớp với mã " << maLop << " đã tồn tại." << endl;
        return false;
    }
    
    if(tenLop.empty()) {
        cout << "Tên lớp không được để trống!" << endl;
        return false;
    }
    
    danhSachLop[maLop] = Lop(maLop, tenLop);
    cout << "Thêm lớp " << tenLop << " (mã: " << maLop << ") thành công." << endl;
    return true;
}

// Thêm sinh viên
bool QuanLyLop::themSinhvien(int maLop, const Sinhvien& sv) {
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
    return true;
}

// Điểm danh lớp
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
            cin.ignore();
            if(choice == 'y' || choice == 'Y') {
                return diemDanhLai(ddNgay, lopIt->second);
            }
            return false; // Người dùng không muốn điểm danh lại
        }
    }
    
    // Nếu chưa có, thực hiện điểm danh mới
    return diemDanhMoi(ngay, lopIt->second);
}

// Sửa điểm danh
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

// Xuất bảng điểm danh theo ngày
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
                        vang++;
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

// Xuất bảng điểm danh của sinh viên
void QuanLyLop::xuatDiemDanhSV(int maLop, int maSV) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << endl;
        return;
    }

    if(!lopIt->second.danhSachSV.count(maSV)) {
        cout << "Sinh viên " << maSV << " không tồn tại trong lớp " << maLop << "!" << endl;
        return;
    }
    cout << "\nBẢNG ĐIỂM DANH CỦA SINH VIÊN " << lopIt->second.danhSachSV[maSV].hoTen << " (MSV: " << maSV << ")" << endl;
    cout << string(60, '=') << endl;
    int soVang = 0, soCoMat = 0;
    for(const auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.trangThai.count(maSV)) {
            cout << "Ngày: " << ddNgay.ngay << " - Trạng thái: " << (ddNgay.trangThai.at(maSV) == 1 ? "Có mặt" : "Vắng") << endl;
            if(ddNgay.trangThai.at(maSV) != 1) soVang++;
            else soCoMat++;
        }
    }
    cout << "Tổng số buổi vắng: " << soVang << endl;
    cout << "Tổng số buổi có mặt: " << soCoMat << endl;
    cout << "Tỷ lệ điểm danh: " << fixed << setprecision(2) << (static_cast<double>(soCoMat) / lopIt->second.danhSachDiemDanh.size()) * 100.0 << "%" << endl;
}

// Xuất lịch sử điểm danh
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

// Xuất thống kê lớp
void QuanLyLop::xemThongKe(int maLop) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        cout << "Lop " << maLop << " khong ton tai trong bo nho!" << endl;
        return;
    }
    
    ThongKe::ThongKeLop tk = ThongKe::tinhThongKe(lopIt->second);
    
    cout << "\nTHỐNG KÊ LỚP " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << endl;
    cout << string(60, '=') << endl;
    cout << "Tổng số sinh viên: " << tk.tongSinhkhoa << endl;
    cout << "Tổng số buổi điểm danh: " << tk.tongBuoiDiemDanh << endl;
    cout << "Tỷ lệ điểm danh trung bình toàn lớp: " << fixed << setprecision(2) 
              << tk.tiLeDiemDanhTrungBinh << "%" << endl;
    
    if (tk.tongSinhkhoa > 0 && tk.tongBuoiDiemDanh > 0) {
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '=') << endl;
        
        cout << left << setw(25) << "Ho va ten" << left << setw(12) << "Ma SV";
        for(const auto& ngay : lopIt->second.danhSachDiemDanh) {
            cout << left << setw(12) << ngay.ngay;
        }
        cout << left << setw(10) << "Tong" << endl;
        
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '-') << endl;
        
        for(const auto& svPair : tk.tongketSinhvien) {
            cout << left << setw(25) << svPair.second.hoTen << left << setw(12) << svPair.first;
            for(const auto& ngay : tk.tongketSinhvien[svPair.first].trangThai) {
                string trangThaiStr = (ngay.second == 1) ? "Co mat" : "Vang";
                cout << left << setw(12) << trangThaiStr;
            }
            cout << left << setw(10) << tk.tongketSinhvien[svPair.first].tongSoLuotCoMat << endl;
        }
        
        cout << string(30 + 12 * tk.tongBuoiDiemDanh + 10, '=') << endl;
    }
    else if (tk.tongSinhkhoa == 0) {
        cout << "Lớp chưa có sinh viên." << endl;
    } else {
        cout << "Lớp chưa có buổi điểm danh nào." << endl;
    }
}

// Lưu điểm danh
void QuanLyLop::saveDiemDanh() {
    if(!FileManager::createDirectory()) {
        cout << "Lỗi: Không thể tạo hoặc truy cập thư mục lưu trữ." << endl;
        return;
    }
    
    if (danhSachLop.empty()) {
        cout << "Không có dữ liệu lớp nào trong bộ nhớ để lưu." << endl;
        return;
    }

    int countSaved = 0;
    for(const auto& lopPair : danhSachLop) {
        if(FileManager::saveLop(lopPair.second)) {
            countSaved++;
        }
    }
    
    cout << "Đã lưu thành công dữ liệu cho " << countSaved << "/" << danhSachLop.size() << " lớp." << endl;
}

// Đọc file
bool QuanLyLop::docFile(int maLopDoc) {
    if(!FileManager::fileExists(maLopDoc)) {
        cout << "File không tồn tại cho lớp " << maLopDoc << "." << endl;
        return false;
    }

    Lop lopDoc;
    if(FileManager::readLop(maLopDoc, lopDoc)) {
        danhSachLop[maLopDoc] = lopDoc;
        cout << "Đọc dữ liệu cho lớp " << maLopDoc << " - '" << lopDoc.tenLop << "' thành công." << endl;
        return true;
    }
    
    cout << "Lỗi: Không thể đọc hoặc xử lý file cho lớp " << maLopDoc << "." << endl;
    return false;
}

// Kiểm tra xem lớp có tồn tại không
bool QuanLyLop::lopTonTai(int maLop) const {
    return danhSachLop.count(maLop);
}

// Lấy số lượng sinh viên
int QuanLyLop::getSoLuongSinhvien(int maLop) const {
    auto it = danhSachLop.find(maLop);
    return (it != danhSachLop.end()) ? it->second.danhSachSV.size() : 0;
}

// Lấy danh sách mã lớp
vector<int> QuanLyLop::getDanhSachMaLop() const {
    vector<int> result;
    result.reserve(danhSachLop.size());
    for(const auto& pair : danhSachLop) {
        result.push_back(pair.first);
    }
    return result;
} 