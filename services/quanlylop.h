#ifndef QUANLYLOP_H
#define QUANLYLOP_H

#include "../modules/structure.h"
#include "../utils/datevalidator.h"
#include "../utils/filemanager.h"
#include "thongke.h"

// Lớp quản lý lớp
class QuanLyLop{
private:
    std::unordered_map<int, Lop> danhSachLop; // Key: maLop, Value: Lop
    
    bool diemDanhMoi(const std::string& ngay, Lop& lop);
    bool diemDanhLai(DiemDanh& diemDanh, const Lop& lop);

public:
    bool themLop(int maLop, const std::string& tenLop);
    bool themSinhvien(int maLop, const Sinhvien& sv);
    bool themDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai);
    bool diemDanh(const std::string& ngay, int maLop);
    bool suaDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai);
    void xuatDiemDanh(const std::string& ngay, int maLop);
    void xuatDiemDanhSV(int maLop, int maSV);
    void xemLichSu(int maLop);
    void xemThongKe(int maLop);
    void saveDiemDanh();
    bool docFile(int maLop);
    bool lopTonTai(int maLop) const;
    int getSoLuongSinhvien(int maLop) const;
    vector<int> getDanhSachMaLop() const;
};

#endif