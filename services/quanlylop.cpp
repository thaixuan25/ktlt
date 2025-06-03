#include "quanlylop.h"
#include <iostream> // Cho std::cout, std::cin
#include <fstream>  // Cho std::ofstream, std::ifstream
#include <sstream>  // Cho std::istringstream
#include <iomanip>  // Cho std::setw, std::left, std::fixed, std::setprecision
#include <limits>   // Cho std::numeric_limits

// Không cần using namespace std; ở đây, sẽ dùng std:: trực tiếp

// --- Định nghĩa các phương thức của QuanLyLop ---

// Private helper methods (nếu có, ví dụ từ code gốc)
bool QuanLyLop::diemDanhMoi(const std::string& ngay, Lop& lop) {
    DiemDanh diemDanhMoiObj(ngay); // Đổi tên để tránh trùng với tham số `ngay` của hàm ngoài
    std::cout << "\nĐIỂM DANH LỚP " << lop.tenLop << " - NGÀY " << ngay << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int count = 1;
    for(const auto& svPair : lop.danhSachSV) {
        const auto& sv = svPair.second;
        int trangThai;
        do {
            std::cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
            std::cin >> trangThai;
            if(std::cin.fail() || (trangThai != 0 && trangThai != 1)) {
                std::cout << "Vui lòng nhập 0 hoặc 1!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm sau khi đọc số
        
        diemDanhMoiObj.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    lop.danhSachDiemDanh.push_back(diemDanhMoiObj);
    std::cout << "Điểm danh thành công!" << std::endl;
    return true;
}

bool QuanLyLop::diemDanhLai(DiemDanh& diemDanh, const Lop& lop) {
    std::cout << "\nĐIỂM DANH LẠI - NGÀY " << diemDanh.ngay << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int count = 1;
    for(const auto& svPair : lop.danhSachSV) {
        const auto& sv = svPair.second;
        int trangThai;
        
        do {
            std::cout << count << ". " << sv.hoTen << " (MSV: " << sv.maSV << ") - Trạng thái (1=Có mặt/0=Vắng): ";
            std::cin >> trangThai;
            if(std::cin.fail() || (trangThai != 0 && trangThai != 1)) {
                 std::cout << "Vui lòng nhập 0 hoặc 1!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                trangThai = -1; // Đặt giá trị không hợp lệ để lặp lại
            }
        } while(trangThai != 0 && trangThai != 1);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm
        
        diemDanh.trangThai[sv.maSV] = trangThai;
        count++;
    }
    
    std::cout << "Điểm danh lại thành công!" << std::endl;
    return true;
}

// Public methods
bool QuanLyLop::themLop(int maLop, const std::string& tenLop) {
    if(FileManager::fileExists(maLop)) { // Giả sử file tồn tại nghĩa là lớp đã tồn tại (theo logic cũ)
        std::cout << "Lớp với mã " << maLop << " đã tồn tại (dựa trên file)! Cân nhắc đọc file trước nếu muốn cập nhật." << std::endl;
        // Hoặc kiểm tra trong danhSachLop nếu logic là lớp trong bộ nhớ
        // if(danhSachLop.count(maLop)) {
        //     std::cout << "Lớp với mã " << maLop << " đã tồn tại trong bộ nhớ!" << std::endl;
        //     return false;
        // }
        return false;
    }
    
    if(tenLop.empty()) {
        std::cout << "Tên lớp không được để trống!" << std::endl;
        return false;
    }
    
    danhSachLop[maLop] = Lop(maLop, tenLop);
    std::cout << "Thêm lớp " << tenLop << " (mã: " << maLop << ") vào bộ nhớ thành công. Hãy lưu file để giữ thay đổi." << std::endl;
    return true;
}

bool QuanLyLop::themSinhkhoa(int maLop, const Sinhkhoa& sv) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ! Hãy thêm lớp hoặc đọc từ file trước." << std::endl;
        return false;
    }
    
    if(lopIt->second.danhSachSV.count(sv.maSV)) {
        std::cout << "Sinh viên với mã " << sv.maSV << " đã tồn tại trong lớp " << maLop << "!" << std::endl;
        return false;
    }
    
    lopIt->second.danhSachSV[sv.maSV] = sv;
    // std::cout << "Thêm sinh viên " << sv.hoTen << " vào lớp " << maLop << " thành công." << std::endl; // Có thể bỏ bớt thông báo này nếu quá nhiều
    return true;
}

bool QuanLyLop::themDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai) {
    if(!DateValidator::isValidDate(ngay)) {
        std::cout << "Định dạng ngày '" << ngay << "' không hợp lệ! Vui lòng nhập theo định dạng dd/mm/yyyy." << std::endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return false;
    }
    
    if(!lopIt->second.danhSachSV.count(maSV)) {
        std::cout << "Sinh viên với mã " << maSV << " không tồn tại trong lớp " << maLop << "!" << std::endl;
        return false;
    }
    
    // Tìm ngày điểm danh trong danhSachDiemDanh của lớp
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            ddNgay.trangThai[maSV] = trangThai;
            // std::cout << "Cập nhật điểm danh cho SV " << maSV << " ngày " << ngay << " thành công." << std::endl;
            return true;
        }
    }
    
    // Nếu không tìm thấy ngày, tạo mới bản ghi điểm danh cho ngày đó
    DiemDanh diemDanhMoiNgay(ngay);
    diemDanhMoiNgay.trangThai[maSV] = trangThai;
    lopIt->second.danhSachDiemDanh.push_back(diemDanhMoiNgay);
    // std::cout << "Thêm điểm danh mới cho SV " << maSV << " ngày " << ngay << " thành công." << std::endl;
    return true;
}

bool QuanLyLop::diemDanh(const std::string& ngay, int maLop) {
    if(!DateValidator::isValidDate(ngay)) {
        std::cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << std::endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return false;
    }
    
    if(lopIt->second.danhSachSV.empty()) {
        std::cout << "Lớp " << maLop << " chưa có sinh viên nào!" << std::endl;
        return false;
    }
    
    // Kiểm tra điểm danh đã tồn tại cho ngày này
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            char choice;
            std::cout << "Điểm danh cho lớp " << lopIt->second.tenLop << " ngày " << ngay << " đã tồn tại. Bạn có muốn điểm danh lại không? (y/n): ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm
            if(choice == 'y' || choice == 'Y') {
                return diemDanhLai(ddNgay, lopIt->second);
            }
            return false; // Người dùng không muốn điểm danh lại
        }
    }
    
    // Nếu chưa có, thực hiện điểm danh mới
    return diemDanhMoi(ngay, lopIt->second);
}

bool QuanLyLop::suaDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai) {
    if(!DateValidator::isValidDate(ngay)) {
        std::cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << std::endl;
        return false;
    }
    
    if(trangThai != 0 && trangThai != 1) {
        std::cout << "Trạng thái điểm danh không hợp lệ (phải là 0 hoặc 1)!" << std::endl;
        return false;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return false;
    }
    
    if(!lopIt->second.danhSachSV.count(maSV)) {
        std::cout << "Sinh viên " << maSV << " không tồn tại trong lớp " << maLop << "!" << std::endl;
        return false;
    }
    
    for(auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            if(ddNgay.trangThai.count(maSV)) {
                ddNgay.trangThai[maSV] = trangThai;
                std::cout << "Sửa điểm danh cho sinh viên " << maSV << " ngày " << ngay << " thành công!" << std::endl;
                return true;
            } else {
                std::cout << "Không tìm thấy bản ghi điểm danh cho sinh viên " << maSV << " vào ngày " << ngay << " để sửa." << std::endl;
                return false;
            }
        }
    }
    
    std::cout << "Không tìm thấy điểm danh cho ngày " << ngay << " của lớp " << maLop << " để sửa." << std::endl;
    return false;
}

void QuanLyLop::xuatDiemDanh(const std::string& ngay, int maLop) {
    if(!DateValidator::isValidDate(ngay)) {
        std::cout << "Định dạng ngày '" << ngay << "' không hợp lệ!" << std::endl;
        return;
    }
    
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return;
    }
    
    for(const auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        if(ddNgay.ngay == ngay) {
            std::cout << "\nBẢNG ĐIỂM DANH" << std::endl;
            std::cout << "Lớp: " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << std::endl;
            std::cout << "Ngày: " << ngay << std::endl;
            std::cout << std::string(80, '=') << std::endl;
            std::cout << std::left << std::setw(10) << "Mã SV" << std::setw(30) << "Họ và tên" 
                      << std::setw(20) << "Khoa" << std::setw(15) << "Trạng thái" << std::endl;
            std::cout << std::string(80, '-') << std::endl;
            
            int coMat = 0, vang = 0;
            if (lopIt->second.danhSachSV.empty()) {
                std::cout << "Lớp chưa có sinh viên nào." << std::endl;
            } else {
                for(const auto& svPair : lopIt->second.danhSachSV) {
                    const auto& sv = svPair.second;
                    auto itTrangThai = ddNgay.trangThai.find(sv.maSV);
                    std::string trangThaiOutput = "Vắng (chưa điểm danh)"; // Mặc định

                    if(itTrangThai != ddNgay.trangThai.end()) {
                        trangThaiOutput = (itTrangThai->second == 1) ? "Có mặt" : "Vắng";
                        if(itTrangThai->second == 1) coMat++; else vang++;
                    } else {
                        vang++; // Nếu không có trong bản ghi điểm danh, coi là vắng
                    }
                    
                    std::cout << std::left << std::setw(10) << sv.maSV << std::setw(30) << sv.hoTen 
                              << std::setw(20) << sv.khoa << std::setw(15) << trangThaiOutput << std::endl;
                }
            }
            
            std::cout << std::string(80, '-') << std::endl;
            int tongSV = lopIt->second.danhSachSV.size();
            std::cout << "Tổng số sinh viên: " << tongSV << std::endl;
            std::cout << "Có mặt: " << coMat << ", Vắng: " << vang << std::endl;
            if (tongSV > 0) {
                std::cout << "Tỷ lệ có mặt: " << std::fixed << std::setprecision(1) 
                          << (static_cast<double>(coMat) / tongSV) * 100.0 << "%" << std::endl;
            } else {
                 std::cout << "Tỷ lệ có mặt: 0.0%" << std::endl;
            }
            return;
        }
    }
    
    std::cout << "Không tìm thấy bản ghi điểm danh cho lớp " << maLop << " vào ngày " << ngay << "." << std::endl;
}

void QuanLyLop::xemLichSu(int maLop) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return;
    }
    
    std::cout << "\nLỊCH SỬ ĐIỂM DANH LỚP " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    if (lopIt->second.danhSachDiemDanh.empty()) {
        std::cout << "Chưa có lịch sử điểm danh nào cho lớp này." << std::endl;
        return;
    }

    for(const auto& ddNgay : lopIt->second.danhSachDiemDanh) {
        std::cout << "Ngày: " << ddNgay.ngay;
        int vangCount = 0; int coMatCount = 0;
        int soSvDaDiemDanh = 0;

        // Đếm dựa trên danh sách sinh viên hiện tại của lớp
        for(const auto& svPair : lopIt->second.danhSachSV) {
            auto itTrangThai = ddNgay.trangThai.find(svPair.first);
            if(itTrangThai != ddNgay.trangThai.end()) { // Chỉ tính nếu SV có trong bản ghi điểm danh ngày đó
                soSvDaDiemDanh++;
                if(itTrangThai->second == 1) coMatCount++;
                else vangCount++;
            }
            // Nếu sinh viên có trong lớp nhưng không có trong bản ghi điểm danh của ngày đó
            // thì không tính vào coMat/vang của ngày đó, vì có thể SV được thêm vào lớp sau ngày đó
        }
        // Hoặc, nếu muốn tính tất cả SV trong lớp hiện tại với ngày đó, và mặc định là vắng nếu không có record:
        // int tongSvTrongLop = lopIt->second.danhSachSV.size();
        // coMatCount = 0;
        // for(const auto& sv_pair : lopIt->second.danhSachSV) {
        //     auto it_trangthai = ddNgay.trangThai.find(sv_pair.first);
        //     if(it_trangthai != ddNgay.trangThai.end() && it_trangthai->second == 1) {
        //         coMatCount++;
        //     }
        // }
        // vangCount = tongSvTrongLop - coMatCount;
        // std::cout << " - Tổng số " << tongSvTrongLop << " SV (hiện tại): Có mặt: " << coMatCount << ", Vắng: " << vangCount << std::endl;

        if (soSvDaDiemDanh > 0) {
             std::cout << " - Số SV được điểm danh: " << soSvDaDiemDanh << " (Có mặt: " << coMatCount << ", Vắng: " << vangCount << ")" << std::endl;
        } else if (!lopIt->second.danhSachSV.empty() && !ddNgay.trangThai.empty()) {
            // Có ngày điểm danh, có SV trong lớp, nhưng không SV nào trong lớp khớp với bản ghi điểm danh
            // (có thể là dữ liệu cũ với danh sách SV khác)
            std::cout << " - Bản ghi điểm danh ngày này có " << ddNgay.trangThai.size() << " mục, nhưng không khớp SV hiện tại." << std::endl;
        }
         else {
            std::cout << " - Không có dữ liệu điểm danh cụ thể cho sinh viên vào ngày này hoặc lớp không có SV." << std::endl;
        }
    }
}

void QuanLyLop::xemThongKe(int maLop) {
    auto lopIt = danhSachLop.find(maLop);
    if(lopIt == danhSachLop.end()) {
        std::cout << "Lớp " << maLop << " không tồn tại trong bộ nhớ!" << std::endl;
        return;
    }
    
    ThongKe::ThongKeLop tk = ThongKe::tinhThongKe(lopIt->second);
    
    std::cout << "\nTHỐNG KÊ LỚP " << lopIt->second.tenLop << " (Mã: " << maLop << ")" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Tổng số sinh viên: " << tk.tongSinhkhoa << std::endl;
    std::cout << "Tổng số buổi đã điểm danh: " << tk.tongBuoiDiemDanh << std::endl;
    std::cout << "Tỷ lệ điểm danh trung bình toàn lớp: " << std::fixed << std::setprecision(1) 
              << tk.tiLeDiemDanhTrungBinh << "%" << std::endl;
    
    if (tk.tongSinhkhoa > 0 && tk.tongBuoiDiemDanh > 0) {
        std::cout << "
Chi tiết số lần vắng của sinh viên:" << std::endl;
        std::cout << std::left << std::setw(10) << "Mã SV" << std::setw(30) << "Họ Tên" << "Số lần vắng" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        for(const auto& svPair : lopIt->second.danhSachSV) {
            int maSV = svPair.first;
            std::string hoTen = svPair.second.hoTen;
            int soVang = tk.soLanVang.count(maSV) ? tk.soLanVang.at(maSV) : 0;
            std::cout << std::left << std::setw(10) << maSV << std::setw(30) << hoTen << soVang << std::endl;
        }
    } else if (tk.tongSinhkhoa == 0) {
        std::cout << "Lớp chưa có sinh viên." << std::endl;
    } else {
        std::cout << "Lớp chưa có buổi điểm danh nào." << std::endl;
    }
}

bool QuanLyLop::saveDiemDanh() {
    if(!FileManager::createDirectory()) { // Đảm bảo thư mục tồn tại
        std::cout << "Lỗi: Không thể tạo hoặc truy cập thư mục lưu trữ '" << FileManager::getFilePath(0).substr(0, FileManager::getFilePath(0).find_last_of("/\")) << "'!" << std::endl; // Cố gắng lấy tên thư mục
        return false;
    }
    
    if (danhSachLop.empty()) {
        std::cout << "Không có dữ liệu lớp nào trong bộ nhớ để lưu." << std::endl;
        return true; // Không có gì để lưu, coi như thành công
    }

    int countSaved = 0;
    for(const auto& lopPair : danhSachLop) {
        const auto& lop = lopPair.second;
        std::string filePath = FileManager::getFilePath(lop.maLop);
        std::ofstream file(filePath);
        
        if(!file.is_open()) {
            std::cout << "Lỗi: Không thể mở file để lưu cho lớp " << lop.maLop << " tại đường dẫn: " << filePath << std::endl;
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
        std::cout << "Đã lưu dữ liệu của " << countSaved << " lớp thành công!" << std::endl;
    } else if (!danhSachLop.empty()) {
        std::cout << "Không lưu được dữ liệu của lớp nào (có thể do lỗi mở file)." << std::endl;
        return false;
    }
    return true;
}

bool QuanLyLop::docFile(int maLopDoc) {
    std::string filePath = FileManager::getFilePath(maLopDoc);
    if(!FileManager::fileExists(maLopDoc)) { // Sử dụng maLopDoc thay vì chỉ fileExists()
        std::cout << "File không tồn tại cho lớp " << maLopDoc << " tại: " << filePath << std::endl;
        return false;
    }
    
    std::ifstream file(filePath);
    if(!file.is_open()) {
        std::cout << "Lỗi: Không thể mở file cho lớp " << maLopDoc << " tại: " << filePath << std::endl;
        return false;
    }
    
    // Xóa dữ liệu lớp cũ (nếu có) trong bộ nhớ trước khi đọc từ file
    // Hoặc, bạn có thể quyết định gộp dữ liệu nếu muốn
    danhSachLop.erase(maLopDoc); 
    
    std::string line;
    
    // 1. Đọc thông tin lớp (dòng đầu tiên)
    if(!getline(file, line)) {
        std::cout << "Lỗi: File " << filePath << " rỗng hoặc không thể đọc dòng thông tin lớp." << std::endl;
        file.close();
        return false;
    }
    
    size_t posDash = line.find(" - ");
    if(posDash == std::string::npos) {
        std::cout << "Lỗi: Format dòng thông tin lớp không đúng trong file " << filePath << ". Mong đợi: 'MaLop - TenLop'." << std::endl;
        file.close();
        return false;
    }
    
    std::string maLopStr = line.substr(0, posDash);
    std::string tenLop;
    try {
        // int maLopFromFile = std::stoi(maLopStr); // Không cần vì đã có maLopDoc
        tenLop = line.substr(posDash + 3);
    } catch (const std::exception& e) {
        std::cout << "Lỗi khi xử lý thông tin lớp từ dòng: '" << line << "'. Chi tiết: " << e.what() << std::endl;
        file.close();
        return false;
    }

    // Tạo lớp mới trong danhSachLop
    Lop lopMoi(maLopDoc, tenLop);
    // danhSachLop[maLopDoc] = lopMoi; // Sẽ thêm vào cuối sau khi đọc hết

    // 2. Đọc danh sách sinh viên
    while(getline(file, line) && line != "---DIEMDANH---") {
        if(line.empty()) continue;
        
        std::istringstream iss(line);
        std::string sv_maStr, sv_hoTen, sv_khoa;
        
        if(getline(iss, sv_maStr, '|') && 
           getline(iss, sv_hoTen, '|') && 
           getline(iss, sv_khoa)) {
            try {
                int sv_ma = std::stoi(sv_maStr);
                lopMoi.danhSachSV[sv_ma] = Sinhkhoa(sv_ma, sv_hoTen, sv_khoa);
            } catch(const std::exception& e) {
                std::cout << "Cảnh báo: Bỏ qua dòng sinh viên không hợp lệ: '" << line << "'. Lỗi: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Cảnh báo: Bỏ qua dòng sinh viên có định dạng không đúng: '" << line << "'" << std::endl;
        }
    }

    // 3. Đọc điểm danh (sau dòng "---DIEMDANH---")
    std::string ngayCurrent;
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
                 std::cout << "Cảnh báo: Bỏ qua dòng ngày không hợp lệ: '" << line << "'" << std::endl;
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
            if(dashPosDD != std::string::npos) {
                try {
                    int maSV_dd = std::stoi(line.substr(0, dashPosDD));
                    int trangThai_dd = std::stoi(line.substr(dashPosDD + 1));
                    
                    // Chỉ thêm nếu sinh viên tồn tại trong danh sách sinh viên của lớp
                    if(lopMoi.danhSachSV.count(maSV_dd) && (trangThai_dd == 0 || trangThai_dd == 1)) {
                         diemDanhNgayObj.trangThai[maSV_dd] = trangThai_dd;
                    } else if (!lopMoi.danhSachSV.count(maSV_dd)){
                        std::cout << "Cảnh báo: Bỏ qua điểm danh cho SV " << maSV_dd << " (không có trong lớp) ngày " << ngayCurrent << std::endl;
                    } else {
                        std::cout << "Cảnh báo: Bỏ qua điểm danh cho SV " << maSV_dd << " (trạng thái " << trangThai_dd << " không hợp lệ) ngày " << ngayCurrent << std::endl;
                    }
                } catch(const std::exception& e) {
                    std::cout << "Cảnh báo: Bỏ qua dòng điểm danh không hợp lệ: '" << line << "'. Lỗi: " << e.what() << std::endl;
                }
            } else {
                 std::cout << "Cảnh báo: Bỏ qua dòng điểm danh có định dạng sai: '" << line << "' cho ngày " << ngayCurrent << std::endl;
            }
        }
    }
    // Xử lý ngày cuối cùng nếu file không kết thúc bằng --ENDNGAY-- (mặc dù nên có)
    if(!ngayCurrent.empty() && !diemDanhNgayObj.trangThai.empty()) {
        lopMoi.danhSachDiemDanh.push_back(diemDanhNgayObj);
    }

    danhSachLop[maLopDoc] = lopMoi; // Thêm lớp đã đọc vào map
    file.close();
    std::cout << "Đọc dữ liệu cho lớp " << maLopDoc << " từ file " << filePath << " thành công!" << std::endl;
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

std::vector<int> QuanLyLop::getDanhSachMaLop() const {
    std::vector<int> result;
    result.reserve(danhSachLop.size()); // Cấp phát trước để tránh realloc nhiều lần
    for(const auto& pair : danhSachLop) {
        result.push_back(pair.first);
    }
    return result;
} 