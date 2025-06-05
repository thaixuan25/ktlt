#include "ui/menu.h"
#include <iostream>
using namespace std;

int main(){
    try {
        Menu m;
        m.menu();
    } catch(const exception& e) {
        cout << "Lỗi nghiêm trọng: " << e.what() << endl;
        return 1;
    }
    return 0;
} 