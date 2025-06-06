#include "thongke.h"
// ---Định nghĩa phương thức tĩnh tinhThongKe---

// Hàm tính thống kê
ThongKeLop ThongKe::tinhThongKe(const Lop& lop) {
    ThongKeLop tk;
    tk.tongSinhvien = lop.danhSachSV.size();
    tk.tongBuoiDiemDanh = lop.danhSachDiemDanh.size();
    
    // Khởi tạo soLanVang cho tất cả sinh viên
    for(const auto& svPair : lop.danhSachSV) {
        tk.soLanVang[svPair.first] = 0;
    }

    if(tk.tongBuoiDiemDanh == 0 || tk.tongSinhvien == 0) {
        tk.tiLeDiemDanhTrungBinh = 0.0;
        return tk;
    }    
    int tongSoLuotCoMat = 0;
    
    // Khởi tạo dữ liệu cho tất cả sinh viên
    for(const auto& svPair : lop.danhSachSV) {
        tk.tongketSinhvien[svPair.first].hoTen = svPair.second.hoTen;
        tk.tongketSinhvien[svPair.first].tongSoLuotCoMat = 0;
    }
    
    // Duyệt qua tất cả các ngày điểm danh
    for(const auto& diemDanhNgay : lop.danhSachDiemDanh) {
        for(const auto& svPair : lop.danhSachSV) {
            // Tìm trạng thái điểm danh của sinh viên svPair.first trong ngày diemDanhNgay
            auto itTrangThai = diemDanhNgay.trangThai.find(svPair.first);
            if(itTrangThai != diemDanhNgay.trangThai.end()) {
                if(itTrangThai->second == 1) { // Có mặt
                    tongSoLuotCoMat++;
                    tk.tongketSinhvien[svPair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 1));
                    tk.tongketSinhvien[svPair.first].tongSoLuotCoMat++;
                } else { // Vắng
                    tk.soLanVang[svPair.first]++;
                    tk.tongketSinhvien[svPair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 0));
                }
            } else {
                // Mặc định là vắng nếu không tìm thấy bản ghi điểm danh cho sinh viên này vào ngày này
                tk.soLanVang[svPair.first]++;
                tk.tongketSinhvien[svPair.first].trangThai.push_back(make_pair(diemDanhNgay.ngay, 0));
            }
        }
    }
    
    // Tính tỷ lệ điểm danh trung bình
    // Đảm bảo không chia cho 0 nếu tongBuoiDiemDanh hoặc tongSinhkhoa là 0 (đã check ở trên)
    tk.tiLeDiemDanhTrungBinh = static_cast<double>(tongSoLuotCoMat) / (static_cast<double>(tk.tongSinhvien) * tk.tongBuoiDiemDanh) * 100.0;
    
    return tk;
} 