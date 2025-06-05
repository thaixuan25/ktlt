#include "thongke.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "../modules/defaults.h" // Cần cho Lop, Sinhkhoa, DiemDanh
#include <unordered_map> // Cần cho std::unordered_map
// Không cần <string> ở đây nếu không dùng trực tiếp
using namespace std;
// Định nghĩa phương thức tĩnh tinhThongKe
ThongKe::ThongKeLop ThongKe::tinhThongKe(const Lop& lop) {
    ThongKeLop tk;
    tk.tongSinhkhoa = lop.danhSachSV.size();
    tk.tongBuoiDiemDanh = lop.danhSachDiemDanh.size();
    
    // Khởi tạo soLanVang cho tất cả sinh viên
    for(const auto& sv_pair : lop.danhSachSV) {
        tk.soLanVang[sv_pair.first] = 0;
    }

    if(tk.tongBuoiDiemDanh == 0 || tk.tongSinhkhoa == 0) {
        tk.tiLeDiemDanhTrungBinh = 0.0;
        // Vẫn trả về tk với soLanVang đã khởi tạo (toàn 0)
        return tk;
    }    
    long long tongSoLuotCoMat = 0; // Sử dụng long long để tránh tràn nếu số lượng lớn
    
    // Khởi tạo dữ liệu cho tất cả sinh viên
    for(const auto& sv_pair : lop.danhSachSV) {
        tk.tongketSinhvien[sv_pair.first].hoTen = sv_pair.second.hoTen;
        tk.tongketSinhvien[sv_pair.first].tongSoLuotCoMat = 0;
    }
    
    // Duyệt qua tất cả các ngày điểm danh
    for(const auto& diemDanhNgay : lop.danhSachDiemDanh) {
        for(const auto& sv_pair : lop.danhSachSV) {
            // Tìm trạng thái điểm danh của sinh viên sv_pair.first trong ngày diemDanhNgay
            auto it_trangthai = diemDanhNgay.trangThai.find(sv_pair.first);
            if(it_trangthai != diemDanhNgay.trangThai.end()) {
                if(it_trangthai->second == 1) { // Có mặt
                    tongSoLuotCoMat++;
                    tk.tongketSinhvien[sv_pair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 1));
                    tk.tongketSinhvien[sv_pair.first].tongSoLuotCoMat++;
                } else { // Vắng
                    tk.soLanVang[sv_pair.first]++;
                    tk.tongketSinhvien[sv_pair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 0));
                }
            } else {
                // Mặc định là vắng nếu không tìm thấy bản ghi điểm danh cho sinh viên này vào ngày này
                tk.soLanVang[sv_pair.first]++;
                tk.tongketSinhvien[sv_pair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 0));
            }
        }
    }
    
    // Tính tỷ lệ điểm danh trung bình
    // Đảm bảo không chia cho 0 nếu tongBuoiDiemDanh hoặc tongSinhkhoa là 0 (đã check ở trên)
    tk.tiLeDiemDanhTrungBinh = static_cast<double>(tongSoLuotCoMat) / (static_cast<double>(tk.tongSinhkhoa) * tk.tongBuoiDiemDanh) * 100.0;
    
    return tk;
} 