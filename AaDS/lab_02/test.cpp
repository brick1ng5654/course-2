#include <iostream>

int main() {
    int a = 5;
    int b = 10;
    int sum = a + b; // Добавьте точку останова на этой строке
    std::cout << "Sum: " << sum << std::endl;
    for(int i=0; i<5;i++){
        a++;
    }
    // Добавьте паузу
    std::cout << "Press any key to continue...";
    std::cin.get();
    return 0;
}
