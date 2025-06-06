#ifndef THONGKE_H
#define THONGKE_H

#include <string>
#include <unordered_map>
#include "../modules/structure.h"


class ThongKe {
public:
    // Cấu trúc thống kê lớp
    struct ThongKeLop {
        int tongSinhkhoa;
        int tongBuoiDiemDanh;
        double tiLeDiemDanhTrungBinh;
        std::unordered_map<int, int> soLanVang;
        std::unordered_map<int, TongketSinhvien> tongketSinhvien;
    };
    
    // Tính toán thống kê
    static ThongKeLop tinhThongKe(const Lop& lop);
};

#endif // THONGKE_H
