#ifndef THONGKE_H
#define THONGKE_H

#include <string>
#include <unordered_map>
#include "../modules/defaults.h"
// using namespace std;

class ThongKe {
public:
    // Cấu trúc thống kê lớp
    struct ThongKeLop {
        int tongSinhkhoa;
        int tongBuoiDiemDanh;
        double tiLeDiemDanhTrungBinh;
        std::unordered_map<int, int> soLanVang;
    };
    
    // Tính toán thống kê
    static ThongKeLop tinhThongKe(const Lop& lop);
};

#endif // THONGKE_H
