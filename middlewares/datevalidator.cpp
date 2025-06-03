#include "datevalidator.h"
#include <string> // Cho std::string, std::stoi
#include <vector> // Cho std::vector
#include <chrono> // Cho std::chrono
#include <sstream> // Cho std::stringstream
#include <iomanip> // Cho std::setfill, std::setw
#include <cctype>  // Cho isdigit
#include <ctime>   // Cho localtime

// Định nghĩa các phương thức tĩnh của DateValidator
bool DateValidator::isValidDate(const std::string& date) {
    if(date.length() != 10) return false;
    if(date[2] != '/' || date[5] != '/') return false;
    
    // Kiểm tra các ký tự là số
    for(int i = 0; i < 10; i++){
        if(i == 2 || i == 5) continue;
        if(!isdigit(date[i])) return false;
    }
    
    int day, month, year;
    try {
        day = std::stoi(date.substr(0, 2));
        month = std::stoi(date.substr(3, 2));
        year = std::stoi(date.substr(6, 4));
    } catch (const std::out_of_range& oor) {
        return false; // Lỗi nếu substr ra ngoài phạm vi, hoặc stoi không đổi được
    } catch (const std::invalid_argument& ia) {
        return false; // Lỗi nếu stoi không đổi được
    }

    if(month < 1 || month > 12) return false;
    if(day < 1 || day > 31) return false;
    if(year < 1900 || year > 2100) return false; // Giới hạn năm hợp lý
    
    std::vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
        daysInMonth[1] = 29; // Năm nhuận
    }
    
    return day <= daysInMonth[month - 1];
}

std::string DateValidator::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now_t = std::chrono::system_clock::to_time_t(now);
    // Sử dụng localtime_s trên Windows hoặc localtime_r trên POSIX để an toàn hơn
    // Tuy nhiên, để đơn giản và giữ nguyên code gốc, tạm dùng localtime
    std::tm tm_snapshot = *localtime(&time_now_t);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << tm_snapshot.tm_mday << "/"
       << std::setfill('0') << std::setw(2) << (tm_snapshot.tm_mon + 1) << "/"
       << (tm_snapshot.tm_year + 1900);
    return ss.str();
} 