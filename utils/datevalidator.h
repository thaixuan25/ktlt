#ifndef DATEVALIDATOR_H
#define DATEVALIDATOR_H

#include <string> // Για std::string
// Không cần chrono ở đây nữa, sẽ chuyển sang .cpp
// using namespace std; // Sẽ bỏ hoặc dùng std:: trong .cpp

// Kiểm tra ngày tháng hợp lệ
class DateValidator {
public:
    static bool isValidDate(const std::string& date);
    static std::string getCurrentDate();
};
#endif