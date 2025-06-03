#ifndef QUANLYLOP_H
#define QUANLYLOP_H

#include <unordered_map>
#include <string>
#include <vector>
#include "../modules/defaults.h"
#include "../middlewares/datevalidator.h"
#include "../controllers/filemanager.h"
#include "thongke.h"

class QuanLyLop{
private:
    std::unordered_map<int, Lop> danhSachLop; // Key: maLop, Value: Lop
    
    bool diemDanhMoi(const std::string& ngay, Lop& lop);
    bool diemDanhLai(DiemDanh& diemDanh, const Lop& lop);

public:
    bool themLop(int maLop, const std::string& tenLop);
    bool themSinhkhoa(int maLop, const Sinhkhoa& sv);
    bool themDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai);
    bool diemDanh(const std::string& ngay, int maLop);
    bool suaDiemDanh(const std::string& ngay, int maLop, int maSV, int trangThai);
    void xuatDiemDanh(const std::string& ngay, int maLop);
    void xemLichSu(int maLop);
    void xemThongKe(int maLop);
    bool saveDiemDanh();
    bool docFile(int maLop);
    bool lopTonTai(int maLop) const;
    int getSoLuongSinhkhoa(int maLop) const;
    std::vector<int> getDanhSachMaLop() const;
};

#endif