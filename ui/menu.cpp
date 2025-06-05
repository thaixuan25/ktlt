#include "menu.h"
#include "../middlewares/datevalidator.h" // Cần cho DateValidator::getCurrentDate()
#include "../modules/defaults.h"      // Cần cho Sinhkhoa
#include <iostream>                     // Cần cho cout, cin, endl
#include <string>                       // Cần cho string, getline
#include <limits>                       // Cần cho numeric_limits

using namespace std;
// -- Định nghĩa các phương thức của Menu --

int Menu::readInt(const string& prompt) {
    int value;
    cout << prompt;
    while(!(cin >> value) || cin.peek() != '\n') { // Thêm kiểm tra ký tự thừa
        cout << "Vui lòng nhập một số nguyên hợp lệ: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Đã xử lý trong vòng lặp hoặc sau khi đọc thành công
    return value;
}

void Menu::pauseScreen() {
    cout << "\nNhấn Enter để tiếp tục...";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Cần thiết nếu trước đó là cin >>
    // Nếu trước đó là getline hoặc cin.ignore đã dọn dẹp rồi thì không cần nữa
    // Tuy nhiên, để an toàn, có thể thêm một lần ignore sạch bộ đệm trước khi get
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Đảm bảo bộ đệm sạch
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
        cout << "6. Xem lịch sử các ngày đã điểm danh của lớp" << endl;
        cout << "7. Xem thống kê điểm danh của lớp" << endl;
        cout << "8. Đọc dữ liệu lớp từ file" << endl;
        cout << "0. Thoát chương trình" << endl;
        cout << string(40, '=') << endl;
        
        int choice = readInt("Nhập lựa chọn (0-9): ");
        // cin.ignore() không cần thiết sau readInt đã cải tiến
        
        switch(choice){
            case 1: handleThemLop(); break;
            case 2: handleThemSinhkhoa(); break;
            case 3: handleDiemDanh(); break;
            case 4: handleSuaDiemDanh(); break;
            case 5: handleXuatDiemDanh(); break;
            case 6: handleTimLichSu(); break;
            case 7: handleXemThongKe(); break;
            case 8: handleDocFile(); break;
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm trước getline
    getline(cin, tenLop);
    
    if(tenLop.empty()){
        cout << "Tên lớp không được để trống! Thao tác bị hủy." << endl;
        return;
    }

    if(ql.themLop(maLop, tenLop)) {
        cout << "Đã thêm lớp " << tenLop << " với mã " << maLop << " thành công." << endl;
        ql.saveDiemDanh();
    }
}

void Menu::handleThemSinhkhoa() {
    cout << "\n --- THÊM SINH VIÊN VÀO LỚP ---" << endl;
    int maLop = readInt("Nhập mã lớp cần thêm sinh viên: ");
    
    // Kiểm tra lớp có trong bộ nhớ không (QuanLyLop::lopTonTai giờ kiểm tra bộ nhớ)
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
        
        cout << "Họ và tên: ";
        string hoTen;
        getline(cin, hoTen);
        
        cout << "Khoa: ";
        string khoa;
        getline(cin, khoa);
        
        if(hoTen.empty() || khoa.empty()) {
            cout << "Thông tin họ tên hoặc khoa không được để trống! Sinh viên này sẽ được bỏ qua." << endl;
            continue; // Bỏ qua sinh viên này, tiếp tục với sinh viên tiếp theo
        }
        
        if(ql.themSinhkhoa(maLop, Sinhkhoa(maSV, hoTen, khoa))) {
            cout << "Đã thêm sinh viên: " << hoTen << " (MSV: " << maSV << ") vào lớp " << maLop << endl;
            addedCount++;
        } else {
            // Thông báo lỗi đã có trong ql.themSinhkhoa (ví dụ: SV đã tồn tại)
            // Có thể không cần thông báo thêm ở đây nếu ql đã đủ rõ ràng
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
    // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Có thể cần nếu readInt trước đó không dọn sạch hoàn toàn
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
        // Thông báo đã có trong ql.diemDanh
        ql.saveDiemDanh(); // Không tự động lưu
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
    if (ql.docFile(maLop)) {
        cout << "Đọc dữ liệu lớp " << maLop << " từ file thành công." << endl;
    }
} 