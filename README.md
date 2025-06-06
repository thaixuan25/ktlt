# Quản Lý Điểm Danh Sinh Viên

![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)
![Standard](https://img.shields.io/badge/Standard-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

Đây là đồ án môn học Kỹ thuật Lập trình, xây dựng một ứng dụng console (CLI) bằng ngôn ngữ C++ để giải quyết bài toán quản lý chuyên cần của sinh viên trong các lớp học. Ứng dụng cho phép quản lý lớp, sinh viên, thực hiện điểm danh, xem lịch sử và thống kê.

## ✨ Tính năng chính

- [x] **Quản lý Lớp học:** Thêm lớp học mới, xem danh sách các lớp hiện có.
- [x] **Quản lý Sinh viên:** Thêm sinh viên vào một lớp học, xem danh sách sinh viên của lớp.
- [x] **Hệ thống Điểm danh:**
    - Thực hiện điểm danh theo ngày cho từng lớp.
    - Sửa đổi thông tin điểm danh khi có sai sót.
    - Xem lại chi tiết một buổi điểm danh cụ thể.
- [x] **Báo cáo & Thống kê:**
    - Xem lịch sử tất cả các buổi điểm danh của một lớp.
    - Xem báo cáo thống kê chuyên cần chi tiết cho từng lớp (tỷ lệ đi học, số buổi vắng của từng sinh viên).
- [x] **Lưu & Tải dữ liệu:** Tự động lưu và tải dữ liệu từ tệp để đảm bảo tính bền vững giữa các phiên làm việc.

## 📂 Cấu trúc Dự án

Dự án được tổ chức theo kiến trúc module hóa, giúp mã nguồn trở nên rõ ràng, dễ quản lý và bảo trì.

```
/
├── controllers/      # Lớp điều khiển, quản lý các tác vụ phụ trợ như file
│   ├── filemanager.h
│   └── filemanager.cpp
├── middlewares/      # Lớp trung gian, cung cấp các tiện ích như xác thực
│   ├── datevalidator.h
│   └── datevalidator.cpp
├── modules/          # Định nghĩa các cấu trúc dữ liệu cốt lõi (Entities)
│   └── defaults.h
├── services/         # Chứa logic nghiệp vụ (Business Logic) chính của ứng dụng
│   ├── quanlylop.h
│   ├── quanlylop.cpp
│   ├── thongke.h
│   └── thongke.cpp
├── ui/               # Giao diện người dùng (User Interface) trên console
│   ├── menu.h
│   └── menu.cpp
├── main.cpp          # Điểm khởi đầu của chương trình
├── Makefile          # Tệp dùng để biên dịch và quản lý dự án
└── README.md
```

## 🛠️ Công nghệ sử dụng

- **Ngôn ngữ:** C++ (tiêu chuẩn C++17)
- **Trình biên dịch:** MinGW g++
- **Hệ thống Build:** GNU Make

## 🚀 Bắt đầu

### Yêu cầu tiên quyết

- Bạn cần cài đặt **MinGW** (cung cấp trình biên dịch `g++` và công cụ `make`) và đảm bảo đã thêm nó vào biến môi trường `PATH` của hệ thống.

### Cài đặt và Chạy

1.  **Clone repository về máy của bạn:**
    ```sh
    git clone <your-repository-url>
    ```

2.  **Di chuyển vào thư mục gốc của dự án:**
    ```sh
    cd <project-folder>
    ```

3.  **Biên dịch dự án:**
    Mở terminal và chạy lệnh `make`. Lệnh này sẽ đọc `Makefile` và biên dịch tất cả các tệp nguồn cần thiết để tạo ra tệp thực thi `main.exe`.
    ```sh
    make
    ```
    Hoặc có thể dùng lệnh `make all`.

4.  **Chạy ứng dụng:**
    Sau khi biên dịch thành công, bạn có thể chạy ứng dụng bằng một trong hai cách sau:
    - Chạy trực tiếp tệp thực thi:
      ```sh
      ./main.exe
      ```
    - Hoặc sử dụng target `run` trong Makefile (lệnh này sẽ tự động biên dịch nếu cần và sau đó chạy):
      ```sh
      make run
      ```

5.  **Dọn dẹp:**
    Để xóa các tệp đã được biên dịch (`main.exe`), sử dụng lệnh:
    ```sh
    make clean
    ```

## 📈 Hướng phát triển trong tương lai

- [ ] Xây dựng giao diện đồ họa (GUI) bằng các thư viện như Qt hoặc ImGui.
- [ ] Thêm chức năng xuất báo cáo, thống kê ra các định dạng tệp như `.csv` hoặc `.pdf`.
- [ ] Cải thiện cơ chế lưu trữ dữ liệu (ví dụ: sử dụng định dạng JSON, hoặc một CSDL nhẹ như SQLite).
- [ ] Viết Unit Test cho các module nghiệp vụ để đảm bảo tính đúng đắn.
- [ ] Thêm hệ thống xác thực người dùng (đăng nhập, phân quyền).

## 📄 Giấy phép

Dự án này được cấp phép theo Giấy phép MIT.

---

_Đồ án được thực hiện trong khuôn khổ môn học Kỹ thuật Lập trình._ 