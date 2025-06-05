#include "datevalidator.h"
#include <string> // Cho string, stoi
#include <vector> // Cho vector
#include <chrono> // Cho chrono
#include <sstream> // Cho stringstream
#include <iomanip> // Cho setfill, setw
#include <cctype>  // Cho isdigit
#include <ctime>   // Cho localtime

using namespace std;
// Định nghĩa các phương thức tĩnh của DateValidator
bool DateValidator::isValidDate(const string& date) {
    if(date.length() != 10) return false;
    if(date[2] != '/' || date[5] != '/') return false;
    
    // Kiểm tra các ký tự là số
    for(int i = 0; i < 10; i++){
        if(i == 2 || i == 5) continue;
        if(!isdigit(date[i])) return false;
    }
    
    int day, month, year;
    try {
        day = stoi(date.substr(0, 2));
        month = stoi(date.substr(3, 2));
        year = stoi(date.substr(6, 4));
    } catch (const out_of_range& oor) {
        return false; // Lỗi nếu substr ra ngoài phạm vi, hoặc stoi không đổi được
    } catch (const invalid_argument& ia) {
        return false; // Lỗi nếu stoi không đổi được
    }

    if(month < 1 || month > 12) return false;
    if(day < 1 || day > 31) return false;
    if(year < 1900 || year > 2100) return false; // Giới hạn năm hợp lý
    
    vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
        daysInMonth[1] = 29; // Năm nhuận
    }
    
    return day <= daysInMonth[month - 1];
}

string DateValidator::getCurrentDate() {
    auto now = chrono::system_clock::now();
    time_t timeNowT = chrono::system_clock::to_time_t(now);
    // Sử dụng localtime_s trên Windows hoặc localtime_r trên POSIX để an toàn hơn
    // Tuy nhiên, để đơn giản và giữ nguyên code gốc, tạm dùng localtime
    tm tmSnapshot = *localtime(&timeNowT);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << tmSnapshot.tm_mday << "/"
       << setfill('0') << setw(2) << (tmSnapshot.tm_mon + 1) << "/"
       << (tmSnapshot.tm_year + 1900);
    return ss.str();
} 