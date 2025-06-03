#ifndef MENU_H
#define MENU_H

#include "../services/quanlylop.h"
#include <string> // Cần cho std::string trong readInt
// #include <iostream> // Sẽ chuyển sang .cpp
// using namespace std; // Sẽ bỏ

class Menu{
private:
    QuanLyLop ql;
    
    // Khai báo các phương thức private helper
    int readInt(const std::string& prompt = "");
    void pauseScreen();
    
    // Khai báo các handler
    void handleThemLop();
    void handleThemSinhkhoa();
    void handleDiemDanh();
    void handleSuaDiemDanh();
    void handleXuatDiemDanh();
    void handleTimLichSu();
    void handleXemThongKe();
    void handleDocFile();

public:
    void menu();
};

#endif