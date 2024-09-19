#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Array of bytes

void input_in_vector(std::vector<int> &inputed, const std::string &name){ // Enter the data of array (vector)
    std::string input;
    std::cout<<"Enter the numbers of " << name << " separated by space: ";
    std::getline(std::cin, input);

    std::stringstream ss(input);
    int number;

    while(ss >> number){
        inputed.push_back(int(number));
    }
}

void association(std::vector<bool>& result, const std::vector<bool>& additional) { // Association of 2 sets, result of operation contain in the first set
    for (int i = 0; i < 10; i++) {
        result[i] = result[i] | additional[i];
    }
}

void intersection(std::vector<bool>& result, const std::vector<bool>& additional){ // Intersection of 2 sets, result of operation contain in the first set
    for(int i=0; i < 10; i++){
        result[i] = result[i] & additional[i];
    }
}

std::vector<bool> array_to_bit_array(const std::vector<int>& array) { // Reform default array to array of bytes
    std::vector<bool> bit_array(10, false); // Initialize bit array with false values
    for (int value : array) {
        bit_array[value] = true;
    }
    return bit_array;
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
    std::vector<int> A, B, C, D, E;
    std::vector<bool> bE;
    std::vector<bool> bA(10);
    std::vector<bool> bB(10);
    std::vector<bool> bC(10);
    std::vector<bool> bD(10);
    // Enter the data
    input_in_vector(A, "A"); // 1, 2
    input_in_vector(B, "B"); // 4, 7
    input_in_vector(C, "C"); // 3, 5
    input_in_vector(D, "D"); // 5, 8, 9
    // Reform to array of bytes
    bA = array_to_bit_array(A);
    bB = array_to_bit_array(B);
    bC = array_to_bit_array(C);
    bD = array_to_bit_array(D);
    // Algorithm
    bE = bA;
    association(bE, bB);
    intersection(bC, bD);
    association(bE, bC);
    
    print_bit_array(bE);
    return 0;
}