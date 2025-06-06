# Quản Lý Điểm Danh Sinh Viên

Đây là báo cáo môn học Kỹ thuật Lập trình, xây dựng một ứng dụng console (CLI) bằng ngôn ngữ C++ để giải quyết bài toán quản lý chuyên cần của sinh viên trong các lớp học. Ứng dụng cho phép quản lý lớp, sinh viên, thực hiện điểm danh, xem lịch sử và thống kê.

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
├── utils/      # Lớp trung gian, cung cấp các tiện ích như xác thực
│   ├── filemanager.h
│   └── filemanager.cpp
│   ├── datevalidator.h
│   └── datevalidator.cpp
├── modules/          # Định nghĩa các cấu trúc dữ liệu cốt lõi
│   └── defaults.h
├── services/         # Chứa logic nghiệp vụ chính của ứng dụng
│   ├── quanlylop.h
│   ├── quanlylop.cpp
│   ├── thongke.h
│   └── thongke.cpp
├── ui/               # Giao diện người dùng trên console
│   ├── menu.h
│   └── menu.cpp
├── main.cpp          # Điểm khởi đầu của chương trình
└── README.md         # Hướng dẫn sử dụng
```

## 🛠️ Công nghệ sử dụng

- **Ngôn ngữ:** C++ (tiêu chuẩn C++17)

## 🚀 Bắt đầu

### Yêu cầu tiên quyết

- Bạn cần cài đặt **MinGW** (cung cấp trình biên dịch `g++`) và đảm bảo đã thêm nó vào biến môi trường `PATH` của hệ thống.

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
    Mở terminal và chạy lệnh `g++ -o main.exe main.cpp -I./utils -I./modules -I./services -I./ui`.
    ```sh
    g++ -o main.exe main.cpp -I./utils -I./modules -I./services -I./ui
    ```

4.  **Chạy ứng dụng:**
    Sau khi biên dịch thành công, bạn có thể chạy ứng dụng bằng một trong hai cách sau:
    - Chạy trực tiếp tệp thực thi:
      ```sh
      ./main.exe
      ```

5.  **Dọn dẹp:**
    Để xóa các tệp đã được biên dịch (`main.exe`), sử dụng lệnh:
    ```sh
    rm main.exe
    ```

## 📈 Hướng phát triển trong tương lai

- [ ] Xây dựng giao diện đồ họa (GUI) bằng các thư viện như Qt hoặc ImGui.
- [ ] Thêm chức năng xuất báo cáo, thống kê ra các định dạng tệp như `.csv` hoặc `.pdf`.
- [ ] Cải thiện cơ chế lưu trữ dữ liệu (ví dụ: sử dụng định dạng JSON, hoặc một CSDL nhẹ như SQLite).
- [ ] Viết Unit Test cho các module nghiệp vụ để đảm bảo tính đúng đắn.
- [ ] Thêm hệ thống xác thực người dùng (đăng nhập, phân quyền).

_Báo cáo được thực hiện trong khuôn khổ môn học Kỹ thuật Lập trình._ 

_Nhóm 222_