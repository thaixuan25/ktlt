#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
// Cấu trúc sinh viên
struct Sinhvien{
    int maSV;
    string hoTen;
    string khoa;
    Sinhvien(int ma = 0, const string& ten = "", const string& k = "") 
        : maSV(ma), hoTen(ten), khoa(k) {}
};

// Cấu trúc điểm danh
struct DiemDanh{
    string ngay;
    unordered_map<int, int> trangThai;
    
    DiemDanh(const string& date = "") : ngay(date) {}
};

// Cấu trúc lớp
struct Lop{
    int maLop;
    string tenLop;
    unordered_map<int, Sinhvien> danhSachSV; // Key: maSV, Value: Sinhvien
    vector<DiemDanh> danhSachDiemDanh;
    
    Lop(int ma = 0, const string& ten = "") : maLop(ma), tenLop(ten) {}
};

// Cấu trúc tổng kết sinh viên
struct TongketSinhvien{
        string hoTen;
        vector<pair<string, int>> trangThai;
        int tongSoLuotCoMat = 0;

        TongketSinhvien(const string& ten = "", const vector<pair<string, int>>& tt = {}, int sl = 0)
        : hoTen(ten), trangThai(tt), tongSoLuotCoMat(sl) {}
};
#endif