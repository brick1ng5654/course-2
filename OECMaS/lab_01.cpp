#include <iostream>
#include <climits>
#include <limits>
#include <string>
#include <cstdint>

union float_union{
    float float_value;
    uint32_t bit_value;
};

int enter_integer(const std::string& message, int a, int b) {
    int input;
    bool flag = false;

    do {
        std::cout << message;
        std::cin >> input;

        if (input >= a && input <= b && std::cin.peek() == '\n') {
            flag = true;
        } else {
            std::cout << "Entered value is not correct. Please try again.Entered value should be in [" << a << ", " << b << "]" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (!flag);

    return input;
}

float enter_float(const std::string& message, float a, float b) {
    float input;
    bool flag = false;

    do {
        std::cout << message;
        std::cin >> input;

        if (input >= a && input <= b && std::cin.peek() == '\n') {
            flag = true;
        } else {
            std::cout << "Entered value is not correct. Please try again. Entered value should be in [" << a << ", " << b << "]" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (!flag);

    return input;
}

void print_unsigned_char(unsigned char value){
    std::cout << "\nValue as symbol: " << value << "\n";
    std::cout << "Value as number: " << static_cast<int>(value) << "\n";
    std::cout << "Value in binary system: ";
    for(int i=sizeof(value)*8-1;i>=0;i--){
        if(value & (1<<i)) std::cout << 1;
        else std::cout << 0;
    }
    std::cout << std::endl;
}

void print_float(float value){
    std::cout << "Value as number: " << value << "\n";
    float_union bits;
    bits.float_value = value;
    for(int i=sizeof(float)*8-1; i>=0;i--){
        if(bits.bit_value & (1<<i)) std::cout << 1;
        else std::cout << 0;
        if(i==31) std::cout << " ";
        else if(i==23) std::cout << " ";
    }

    std::cout << std::endl;
}

unsigned char input_unsigned_char(){
    int input = enter_integer("Enter the value (unsigned char): ", 0, UCHAR_MAX);
    return static_cast<unsigned char>(input);
}

float input_float(){
    float input = enter_float("Etner the value (float): ", std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
    return input;
}

// If in count of changable bits include high order bit then change (1<<count+1) to (1<<count)

void unsigned_char_task(unsigned char &value){
    std::cout << "\n";
    int hob = enter_integer("Enter the number of high-order bit: ", 0, sizeof(char)*8-1);
    int count = enter_integer("Enter the count of changable bits: ", 0, hob);
    int state = enter_integer("Enter the state of bits: ", 0, 1);
    unsigned char mask = ((1<<count+1)-1)<<(hob-count);
    if(state) value |= mask;
    else value &= ~mask;
}

void float_task(float &value){
    std::cout << "\n";
    int hob = enter_integer("Enter the number of high-order bit: ", 0, sizeof(float)*8-1);
    int count = enter_integer("Enter the count of changable bits: ", 0, hob);
    int state = enter_integer("Enter the state of bits: ", 0, 1);
    float_union bits;
    bits.float_value = value;
    int mask = ((1<<count+1)-1)<<(hob-count);
    if(state) bits.bit_value |= mask;
    else bits.bit_value &= ~mask;
    value = bits.float_value;
}

int main() {
    // unsigned char value = input_unsigned_char();
    // print_unsigned_char(value);
    // unsigned_char_task(value);
    // std::cout << "___________\n";
    // print_unsigned_char(value);

    float float_value = input_float();
    print_float(float_value);
    float_task(float_value);
    std::cout << "___________\n";
    print_float(float_value);

    return 0;    
}
