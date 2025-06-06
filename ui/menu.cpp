#include "menu.h"
#include "../utils/datevalidator.h"
#include "../modules/structure.h"
#include <iostream>

using namespace std;
// -- Định nghĩa các phương thức của Menu --

int Menu::readInt(const string& prompt) {
    int value;
    cout << prompt;
    while(!(cin >> value) || cin.peek() != '\n') {
        cout << "Vui lòng nhập một số nguyên hợp lệ: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

void Menu::pauseScreen() {
    cout << "\nNhấn Enter để tiếp tục...";
    cin.ignore();
    cin.get();
}

void Menu::menu(){
    cout << "🎓 === HỆ THỐNG QUẢN LÝ ĐIỂM DANH === 🎓" << endl;
    cout << "Ngày hiện tại: " << DateValidator::getCurrentDate() << endl;
    
    bool running = true;
    while(running){
        cout << "\n" << string(40, '=') << endl;
        cout << "MENU CHÍNH" << endl;
        cout << string(40, '=') << endl;
        cout << "1. Thêm lớp học" << endl;
        cout << "2. Thêm sinh viên vào lớp" << endl;
        cout << "3. Điểm danh" << endl;
        cout << "4. Sửa điểm danh" << endl;
        cout << "5. Xuất bảng điểm danh theo ngày" << endl;
        cout << "6. Xuất bảng điểm danh của sinh viên" << endl;
        cout << "7. Xem lịch sử điểm danh của lớp" << endl;
        cout << "8. Xem thống kê điểm danh của lớp" << endl;
        cout << "9. Đọc dữ liệu lớp từ file" << endl;
        cout << "0. Thoát chương trình" << endl;
        cout << string(40, '=') << endl;
        
        int choice = readInt("Nhập lựa chọn (0-9): ");
        // cin.ignore() không cần thiết sau readInt đã cải tiến
        
        switch(choice){
            case 1: handleThemLop(); break;
            case 2: handleThemSinhvien(); break;
            case 3: handleDiemDanh(); break;
            case 4: handleSuaDiemDanh(); break;
            case 5: handleXuatDiemDanh(); break;
            case 6: handleXuatDiemDanhSV(); break;
            case 7: handleTimLichSu(); break;
            case 8: handleXemThongKe(); break;
            case 9: handleDocFile(); break;
            case 0: 
                cout << "Cảm ơn bạn đã sử dụng hệ thống! Tạm biệt!" << endl;
                running = false;
                break;
            default:
                cout << "Lựa chọn không hợp lệ! Vui lòng chọn lại." << endl;
                break;
        }
        pauseScreen();
    }
}

void Menu::handleThemLop() {
    cout << "\n--- THÊM LỚP HỌC MỚI ---" << endl;
    int maLop = readInt("Nhập mã lớp (số nguyên): ");
    
    cout << "Nhập tên lớp: ";
    string tenLop;
    cin.ignore();
    getline(cin, tenLop);
    
    if(tenLop.empty()){
        cout << "Tên lớp không được để trống! Thao tác bị hủy." << endl;
        return;
    }

    if(ql.themLop(maLop, tenLop)) {
        ql.saveDiemDanh();
    }
}

void Menu::handleThemSinhvien() {
    cout << "\n --- THÊM SINH VIÊN VÀO LỚP ---" << endl;
    int maLop = readInt("Nhập mã lớp cần thêm sinh viên: ");
    
    if(!ql.lopTonTai(maLop)) {
        cout << "Lớp với mã " << maLop << " không tồn tại trong bộ nhớ. Vui lòng thêm lớp hoặc đọc từ file trước." << endl;
        return;
    }

    int n = readInt("Nhập số lượng sinh viên cần thêm: ");
    if(n <= 0) {
        cout << "Số lượng sinh viên không hợp lệ! Thao tác bị hủy." << endl;
        return; 
    }
    
    int addedCount = 0;
    for(int i = 0; i < n; i++) {
        cout << "\n--- Nhập thông tin cho sinh viên thứ " << (i+1) << " ---" << endl;
        int maSV = readInt("Mã sinh viên (số nguyên): ");
        cin.ignore(); // Xóa bộ đệm
        
        cout << "Họ và tên: ";
        string hoTen;
        getline(cin, hoTen);
        
        cout << "Khoa: ";
        string khoa;
        getline(cin, khoa);
        
        if(hoTen.empty() || khoa.empty()) {
            cout << "Thông tin họ tên hoặc khoa không được để trống! Sinh viên này sẽ được bỏ qua." << endl;
            continue;
        }
        
        if(ql.themSinhvien(maLop, Sinhvien(maSV, hoTen, khoa))) {
            addedCount++;
        }
    }
    
    cout << "\nHoàn tất thêm sinh viên. Đã thêm thành công " << addedCount << "/" << n << " sinh viên vào lớp " << maLop << "." << endl;
    if(addedCount > 0) ql.saveDiemDanh();
}

void Menu::handleDiemDanh() {
    cout << "\n --- THỰC HIỆN ĐIỂM DANH ---" << endl;
    string ngayHienTai = DateValidator::getCurrentDate();
    cout << "Ngày hiện tại theo hệ thống: " << ngayHienTai << endl;
    cout << "Nhập ngày cần điểm danh (định dạng dd/mm/yyyy, nhấn Enter để dùng ngày hiện tại): ";
    
    string ngayNhap;
    cin.ignore();
    getline(cin, ngayNhap);

    if(ngayNhap.empty()) {
        ngayNhap = ngayHienTai;
        cout << "Sử dụng ngày hiện tại: " << ngayHienTai << endl;
    }

    if(!DateValidator::isValidDate(ngayNhap)){
        cout << "Định dạng ngày không hợp lệ: '" << ngayNhap << "'. Thao tác bị hủy." << endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp cần điểm danh: ");
    
    if(ql.diemDanh(ngayNhap, maLop)) {
        ql.saveDiemDanh();
    }
}

void Menu::handleSuaDiemDanh() {
    cout << "\n --- SỬA THÔNG TIN ĐIỂM DANH ---" << endl;
    cout << "Nhập ngày điểm danh cần sửa (dd/mm/yyyy): ";
    string ngay;
    cin.ignore();
    getline(cin, ngay);

    if(!DateValidator::isValidDate(ngay)){
        cout << "Định dạng ngày không hợp lệ: '" << ngay << "'. Thao tác bị hủy." << endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp: ");
    int maSV = readInt("Nhập mã sinh viên cần sửa điểm danh: ");
    int trangThai = readInt("Nhập trạng thái mới (1=Có mặt / 0=Vắng): ");
    
    if(trangThai != 0 && trangThai != 1){
        cout << "Trạng thái không hợp lệ. Phải là 0 hoặc 1. Thao tác bị hủy." << endl;
        return;
    }

    if(ql.suaDiemDanh(ngay, maLop, maSV, trangThai)) {
        ql.saveDiemDanh();
    }
}

void Menu::handleXuatDiemDanh() {
    cout << "\n --- XUẤT BẢNG ĐIỂM DANH THEO NGÀY ---" << endl;
    cout << "Nhập ngày cần xuất bảng điểm danh (dd/mm/yyyy): ";
    string ngay;
    cin.ignore();
    getline(cin, ngay);

    if(!DateValidator::isValidDate(ngay)){
        cout << "Định dạng ngày không hợp lệ: '" << ngay << "'. Thao tác bị hủy." << endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp: ");
    ql.xuatDiemDanh(ngay, maLop);
}

void Menu::handleXuatDiemDanhSV() {
    cout << "\n --- XUẤT BẢNG ĐIỂM DANH CỦA SINH VIÊN ---" << endl;
    int maSV = readInt("Nhập mã sinh viên: ");
    int maLop = readInt("Nhập mã lớp: ");
    ql.xuatDiemDanhSV(maLop, maSV);
}

void Menu::handleTimLichSu() {
    cout << "\n --- XEM LỊCH SỬ CÁC NGÀY ĐÃ ĐIỂM DANH ---" << endl;
    int maLop = readInt("Nhập mã lớp cần xem lịch sử: ");
    
    ql.xemLichSu(maLop);
}

void Menu::handleXemThongKe() {
    cout << "\n --- XEM THỐNG KÊ ĐIỂM DANH CỦA LỚP ---" << endl;
    int maLop = readInt("Nhập mã lớp cần xem thống kê: ");
    ql.xemThongKe(maLop);
}

void Menu::handleDocFile() {
    cout << "\n --- ĐỌC DỮ LIỆU LỚP TỪ FILE ---" << endl;
    int maLop = readInt("Nhập mã lớp cần đọc dữ liệu từ file: ");
    ql.docFile(maLop);
} 