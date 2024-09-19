#include <iostream>
#include <climits>

unsigned char checkAndReturnUnsignedChar() {
    int input;  // Используем int для ввода, чтобы захватить значения, выходящие за пределы unsigned char
    bool flag = false;
    do{
        std::cout << "Enter the value: ";
        std::cin >> input;

        // Проверяем, лежит ли значение в допустимых пределах для unsigned char
        if (input >= 0 && input <= UCHAR_MAX) {
            return static_cast<unsigned char>(input);  // Преобразуем в unsigned char
            flag = true;
        } else {
            std::cout << "Entered value is not correct. Please try again!\n";
        }
    } while (!flag);
    return static_cast<unsigned char>(input);
}


int main() {
    unsigned char result = checkAndReturnUnsignedChar();
    std::cout << "Result: " << result << " (as symbol), " << static_cast<int>(result) << " (as number)" << std::endl;
    return 0;
}
