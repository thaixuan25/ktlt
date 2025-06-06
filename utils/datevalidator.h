#ifndef DATEVALIDATOR_H
#define DATEVALIDATOR_H

#include <string>

// Kiểm tra ngày tháng hợp lệ
class DateValidator {
public:
    static bool isValidDate(const std::string& date);
    static std::string getCurrentDate();
};
#endif