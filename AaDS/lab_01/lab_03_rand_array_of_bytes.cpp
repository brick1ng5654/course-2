#include <iostream>
#include <vector>
#include <time.h>
#include <bitset>
#define SIZE 10
#define REPEAT 10000000

// Array of bytes

void association(std::bitset<SIZE>& result, const std::bitset<SIZE>& additional) {
    result |= additional; // Побитовое ИЛИ для объединения
}

void intersection(std::bitset<SIZE>& result, const std::bitset<SIZE>& additional) {
    result &= additional; // Побитовое И для пересечения
}

void print_bit_array(const std::vector<bool>& bit_array) {
    std::vector<bool> bin_code;
    for(int i=0; i<10; i++){
        if(bit_array[i]==1){
            std::cout << i << " ";
        }
    }
    std::cout << "\n\t\t";
    for(int i=9; i>=0; i--){
        if(bit_array[i]==1) std::cout << 1;
        else std::cout << 0;
    }
    std::cout << std::endl;
}

int main() {
    srand(time(0)); // To random values
    int t1 = clock();
    for(int i=0; i<REPEAT; i++){
        std::bitset<SIZE> bA(rand() % 1024); // Генерация случайного битового набора
        std::bitset<SIZE> bB(rand() % 1024);
        std::bitset<SIZE> bC(rand() % 1024);
        std::bitset<SIZE> bD(rand() % 1024);
        std::bitset<SIZE> bE;
        // Algorithm
        bE = bA;
        association(bE, bB);
        intersection(bC, bD);
        association(bE, bC);
    }
    int t2 = clock();
    double t = double(t2-t1) / CLOCKS_PER_SEC;
    std::cout << "Time: " << t;
    
    
    return 0;
}