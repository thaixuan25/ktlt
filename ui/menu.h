#ifndef MENU_H
#define MENU_H

#include "../services/quanlylop.h"
#include "../utils/datevalidator.h"
#include "../modules/structure.h"

// Lớp menu
class Menu{
private:
    QuanLyLop ql;
    int readInt(const std::string& prompt = "");
    void pauseScreen();
    
    // Khai báo các handler
    void handleThemLop();
    void handleThemSinhvien();
    void handleDiemDanh();
    void handleSuaDiemDanh();
    void handleXuatDiemDanh();
    void handleXuatDiemDanhSV();
    void handleTimLichSu();
    void handleXemThongKe();
    void handleDocFile();

public:
    void menu();
};

#endif