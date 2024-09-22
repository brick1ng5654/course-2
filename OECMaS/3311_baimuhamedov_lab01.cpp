#include <iostream>
#include <climits>
#include <limits>
#include <string>
#include <cstdint>

// Union

union float_union{
    float float_value;
    uint32_t bit_value;
};

// Input

int enter_integer(const std::string& message, int a, int b) {
    int input;
    bool flag = false;

    do {
        std::cout << message;
        std::cin >> input; // cause input is int then will be read only first integer part of input

        if (input >= a && input <= b && std::cin.peek() == '\n') { // peek() to check has something else after integer part
            flag = true;
        } else {
            std::cout << "Entered value is not correct. Please try again.Entered value should be in [" << a << ", " << b << "]" << std::endl;
            std::cin.clear(); // reset all flags of errors to accept input again
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore and clear all other symbols
        }
    } while (!flag);

    return input;
}

float enter_float(const std::string& message, float a, float b) {
    float input;
    bool flag = false;

    do {
        std::cout << message;
        std::cin >> input; // cause input is float then will be read only first float part of input

        if (input >= a && input <= b && std::cin.peek() == '\n') { // peek() to check has something else after float part
            flag = true;
        } else {
            std::cout << "Entered value is not correct. Please try again. Entered value should be in [" << a << ", " << b << "]" << std::endl;
            std::cin.clear(); // reset all flags of errors to accept input again
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore and clear all other symbols
        }
    } while (!flag);

    return input;
}

unsigned char input_unsigned_char(){
    int input = enter_integer("Enter the value (unsigned char): ", 0, UCHAR_MAX);
    return static_cast<unsigned char>(input);
}

float input_float(){
    float input = enter_float("Etner the value (float): ", std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
    return input;
}

// Output

void print_unsigned_char(unsigned char value){
    std::cout << "\nValue as symbol: " << value << "\n";
    std::cout << "Value as number: " << static_cast<int>(value) << "\n";
    std::cout << "Value in binary system: ";
    for(int i=sizeof(char)*8-1;i>=0;i--){ // sizeof return value in bytes
        if(value & (1<<i)) std::cout << 1;
        else std::cout << 0;
    }
    std::cout << std::endl;
}

void print_float(float value){
    std::cout << "\nValue as number: " << value << "\n";
    float_union bits;
    bits.float_value = value; // fill `union bits` bites of float value
    std::cout << "Value in binart system: ";
    for(int i=sizeof(float)*8-1; i>=0;i--){ // sizeof return value in bytes
        if(bits.bit_value & (1<<i)) std::cout << 1;
        else std::cout << 0;
        if(i==31) std::cout << " "; // to divide sign bite
        else if(i==23) std::cout << " "; // to divide `p` and `m`
    }

    std::cout << std::endl;
}

// Task

void unsigned_char_task(unsigned char &value){
    std::cout << "\n";
    int hob = enter_integer("Enter the number of high-order bit: ", 0, sizeof(char)*8-1); // for example 4
    int count = enter_integer("Enter the count of changable bits: ", 0, hob); // for example 2
    int state = enter_integer("Enter the state of bits: ", 0, 1); // for example 1
    unsigned char mask = ((1<<count+1)-1)<<(hob-count); //step by step: 00000001 -> 00001000 -> 00000111 -> 00011100  
    if(state) value |= mask;
    else value &= ~mask; // if state equial zero, then mask transfrom to 11100011
}

void float_task(float &value){
    std::cout << "\n";
    int hob = enter_integer("Enter the number of high-order bit: ", 0, sizeof(float)*8-1); // for exmaple 4
    int count = enter_integer("Enter the count of changable bits: ", 0, hob); // for example 2
    int state = enter_integer("Enter the state of bits: ", 0, 1); // for example 1
    float_union bits;
    bits.float_value = value; // fill `union bits` bites of float value
    int mask = ((1<<count+1)-1)<<(hob-count); //step by step: 000...00001 -> 000...01000 -> 000...00111 -> 000...11100  
    if(state) bits.bit_value |= mask;
    else bits.bit_value &= ~mask; // invert mask
    value = bits.float_value; // bites of `value` take bites of `union bits`
}

int main(){
    int option=0;
    do{
        std::cout << "Choose the option:\n0 - for exit\n1 - for unsigned char\n2 - for float\n";
        option = enter_integer("Enter the option: ",0, 2);
        switch(option){
            case 1: {
                unsigned char uchar_value = input_unsigned_char();
                print_unsigned_char(uchar_value);
                unsigned_char_task(uchar_value);
                print_unsigned_char(uchar_value);
                std::cout << "\n__________\n\n";
                break;
            }
            case 2: {
                float float_value = input_float();
                print_float(float_value);
                float_task(float_value);
                print_float(float_value);
                std::cout << "\n__________\n\n";
                break;
            }
            case 0:
                option = 0;
                break;
            default:
                break;
        }
    } while(option!=0);
    return 0;
}