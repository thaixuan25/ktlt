#include "menu.h"
#include "../middlewares/datevalidator.h" // Cần cho DateValidator::getCurrentDate()
#include "../modules/defaults.h"      // Cần cho Sinhkhoa
#include <iostream>                     // Cần cho std::cout, std::cin, std::endl
#include <string>                       // Cần cho std::string, std::getline
#include <limits>                       // Cần cho std::numeric_limits

// -- Định nghĩa các phương thức của Menu --

int Menu::readInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while(!(std::cin >> value) || std::cin.peek() != '\n') { // Thêm kiểm tra ký tự thừa
        std::cout << "Vui lòng nhập một số nguyên hợp lệ: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Đã xử lý trong vòng lặp hoặc sau khi đọc thành công
    return value;
}

void Menu::pauseScreen() {
    std::cout << "\nNhấn Enter để tiếp tục...";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Cần thiết nếu trước đó là cin >>
    // Nếu trước đó là getline hoặc cin.ignore đã dọn dẹp rồi thì không cần nữa
    // Tuy nhiên, để an toàn, có thể thêm một lần ignore sạch bộ đệm trước khi get
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Đảm bảo bộ đệm sạch
    std::cin.get();
}

void Menu::menu(){
    std::cout << "🎓 === HỆ THỐNG QUẢN LÝ ĐIỂM DANH === 🎓" << std::endl;
    std::cout << "Ngày hiện tại: " << DateValidator::getCurrentDate() << std::endl;
    
    bool running = true;
    while(running){
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "MENU CHÍNH" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "1. Thêm lớp học" << std::endl;
        std::cout << "2. Thêm sinh viên vào lớp" << std::endl;
        std::cout << "3. Điểm danh" << std::endl;
        std::cout << "4. Sửa điểm danh" << std::endl;
        std::cout << "5. Xuất bảng điểm danh theo ngày" << std::endl;
        std::cout << "6. Xem lịch sử các ngày đã điểm danh của lớp" << std::endl;
        std::cout << "7. Xem thống kê điểm danh của lớp" << std::endl;
        std::cout << "8. Đọc dữ liệu lớp từ file" << std::endl;
        std::cout << "9. Lưu tất cả dữ liệu hiện tại vào file" << std::endl;
        std::cout << "0. Thoát chương trình" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        
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
            case 9: 
                if(ql.saveDiemDanh()) {
                    // std::cout << "Dữ liệu đã được lưu thành công!" << std::endl; // Thông báo đã có trong saveDiemDanh
                } else {
                    std::cout << "Có lỗi xảy ra khi lưu dữ liệu." << std::endl;
                }
                break;
            case 0: 
                std::cout << "Cảm ơn bạn đã sử dụng hệ thống! Tạm biệt!" << std::endl;
                running = false;
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ! Vui lòng chọn lại." << std::endl;
                break;
        }
        
        if(running) {
            pauseScreen();
        }
    }
}

void Menu::handleThemLop() {
    std::cout << "\n✏️ --- THÊM LỚP HỌC MỚI ---" << std::endl;
    int maLop = readInt("Nhập mã lớp (số nguyên): ");
    
    std::cout << "Nhập tên lớp: ";
    std::string tenLop;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm trước getline
    std::getline(std::cin, tenLop);
    
    if(tenLop.empty()){
        std::cout << "Tên lớp không được để trống! Thao tác bị hủy." << std::endl;
        return;
    }

    if(ql.themLop(maLop, tenLop)) {
        // Thông báo đã có trong ql.themLop
        // Cân nhắc hỏi người dùng có muốn lưu ngay không, hoặc để lưu tổng thể ở menu chính
        // ql.saveDiemDanh(); // Tạm thời không tự động lưu ở đây, để người dùng chọn lưu ở menu
    }
}

void Menu::handleThemSinhkhoa() {
    std::cout << "\n👥 --- THÊM SINH VIÊN VÀO LỚP ---" << std::endl;
    int maLop = readInt("Nhập mã lớp cần thêm sinh viên: ");
    
    // Kiểm tra lớp có trong bộ nhớ không (QuanLyLop::lopTonTai giờ kiểm tra bộ nhớ)
    if(!ql.lopTonTai(maLop)) {
        std::cout << "Lớp với mã " << maLop << " không tồn tại trong bộ nhớ. Vui lòng thêm lớp hoặc đọc từ file trước." << std::endl;
        return;
    }

    int n = readInt("Nhập số lượng sinh viên cần thêm: ");
    if(n <= 0) {
        std::cout << "Số lượng sinh viên không hợp lệ! Thao tác bị hủy." << std::endl;
        return;
    }
    
    int addedCount = 0;
    for(int i = 0; i < n; i++) {
        std::cout << "\n--- Nhập thông tin cho sinh viên thứ " << (i+1) << " ---" << std::endl;
        int maSV = readInt("Mã sinh viên (số nguyên): ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm
        
        std::cout << "Họ và tên: ";
        std::string hoTen;
        std::getline(std::cin, hoTen);
        
        std::cout << "Khoa: ";
        std::string khoa;
        std::getline(std::cin, khoa);
        
        if(hoTen.empty() || khoa.empty()) {
            std::cout << "Thông tin họ tên hoặc khoa không được để trống! Sinh viên này sẽ được bỏ qua." << std::endl;
            continue; // Bỏ qua sinh viên này, tiếp tục với sinh viên tiếp theo
        }
        
        if(ql.themSinhkhoa(maLop, Sinhkhoa(maSV, hoTen, khoa))) {
            std::cout << "Đã thêm sinh viên: " << hoTen << " (MSV: " << maSV << ") vào lớp " << maLop << std::endl;
            addedCount++;
        } else {
            // Thông báo lỗi đã có trong ql.themSinhkhoa (ví dụ: SV đã tồn tại)
            // Có thể không cần thông báo thêm ở đây nếu ql đã đủ rõ ràng
        }
    }
    
    std::cout << "\nHoàn tất thêm sinh viên. Đã thêm thành công " << addedCount << "/" << n << " sinh viên vào lớp " << maLop << "." << std::endl;
    // if(addedCount > 0) ql.saveDiemDanh(); // Không tự động lưu
}

void Menu::handleDiemDanh() {
    std::cout << "\n📅 --- THỰC HIỆN ĐIỂM DANH ---" << std::endl;
    std::string ngayHienTai = DateValidator::getCurrentDate();
    std::cout << "Ngày hiện tại theo hệ thống: " << ngayHienTai << std::endl;
    std::cout << "Nhập ngày cần điểm danh (định dạng dd/mm/yyyy, nhấn Enter để dùng ngày hiện tại): ";
    
    std::string ngayNhap;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Có thể cần nếu readInt trước đó không dọn sạch hoàn toàn
    std::getline(std::cin, ngayNhap);

    if(ngayNhap.empty()) {
        ngayNhap = ngayHienTai;
        std::cout << "Sử dụng ngày hiện tại: " << ngayHienTai << std::endl;
    }

    if(!DateValidator::isValidDate(ngayNhap)){
        std::cout << "Định dạng ngày không hợp lệ: '" << ngayNhap << "'. Thao tác bị hủy." << std::endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp cần điểm danh: ");
    
    if(ql.diemDanh(ngayNhap, maLop)) {
        // Thông báo đã có trong ql.diemDanh
        // ql.saveDiemDanh(); // Không tự động lưu
    }
}

void Menu::handleSuaDiemDanh() {
    std::cout << "\n✍️ --- SỬA THÔNG TIN ĐIỂM DANH ---" << std::endl;
    std::cout << "Nhập ngày điểm danh cần sửa (dd/mm/yyyy): ";
    std::string ngay;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, ngay);

    if(!DateValidator::isValidDate(ngay)){
        std::cout << "Định dạng ngày không hợp lệ: '" << ngay << "'. Thao tác bị hủy." << std::endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp: ");
    int maSV = readInt("Nhập mã sinh viên cần sửa điểm danh: ");
    int trangThai = readInt("Nhập trạng thái mới (1=Có mặt / 0=Vắng): ");
    
    if(trangThai != 0 && trangThai != 1){
        std::cout << "Trạng thái không hợp lệ. Phải là 0 hoặc 1. Thao tác bị hủy." << std::endl;
        return;
    }

    if(ql.suaDiemDanh(ngay, maLop, maSV, trangThai)) {
        // Thông báo đã có trong ql.suaDiemDanh
        // ql.saveDiemDanh(); // Không tự động lưu
    }
}

void Menu::handleXuatDiemDanh() {
    std::cout << "\n📋 --- XUẤT BẢNG ĐIỂM DANH THEO NGÀY ---" << std::endl;
    std::cout << "Nhập ngày cần xuất bảng điểm danh (dd/mm/yyyy): ";
    std::string ngay;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, ngay);

    if(!DateValidator::isValidDate(ngay)){
        std::cout << "Định dạng ngày không hợp lệ: '" << ngay << "'. Thao tác bị hủy." << std::endl;
        return;
    }
        
    int maLop = readInt("Nhập mã lớp: ");
    ql.xuatDiemDanh(ngay, maLop);
}

void Menu::handleTimLichSu() {
    std::cout << "\n📜 --- XEM LỊCH SỬ CÁC NGÀY ĐÃ ĐIỂM DANH ---" << std::endl;
    int maLop = readInt("Nhập mã lớp cần xem lịch sử: ");
    // int maSV = readInt("Nhập mã sinh viên (nhập 0 nếu muốn xem cả lớp): "); // Logic cũ chỉ xem theo lớp, giữ nguyên
    
    ql.xemLichSu(maLop);
}

void Menu::handleXemThongKe() {
    std::cout << "\n📊 --- XEM THỐNG KÊ ĐIỂM DANH CỦA LỚP ---" << std::endl;
    int maLop = readInt("Nhập mã lớp cần xem thống kê: ");
    ql.xemThongKe(maLop);
}

void Menu::handleDocFile() {
    std::cout << "\n💾 --- ĐỌC DỮ LIỆU LỚP TỪ FILE ---" << std::endl;
    int maLop = readInt("Nhập mã lớp cần đọc dữ liệu từ file: ");
    if (ql.docFile(maLop)) {
        // Thông báo đã có trong ql.docFile
    } else {
        // Thông báo lỗi đã có trong ql.docFile
    }
} 