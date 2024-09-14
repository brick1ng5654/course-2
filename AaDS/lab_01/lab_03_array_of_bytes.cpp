#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Array of bytes

int max_element_of_array(const std::vector<int>& array){
    int max = array[0];
    for(int i=0; i<array.size(); i++ ){
        if (array[i]>max){max = array[i];}
    }
    return max;
}

int max_of_array_size(int size1, int size2){
    return (size1 > size2) ? size1 : size2;
}

int min_of_array_size(int size1, int size2){
    return (size1 > size2) ? size2 : size1;
}

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

bool is_in_set(int element, const std::vector<int>& set){ // Is element contain in set?
    for(int i=0; i<set.size();i++){
        if(element == set[i]) return true;
    }
    return false;
}

void association(std::vector<bool>& result, const std::vector<bool>& additional) { // Association of 2 sets, result of operation contain in the first set
    int max_size = max_of_array_size(result.size(), additional.size());
    result.resize(max_size, false); // Resize result to the maximum size
    for (int i = 0; i < max_size; i++) {
        if (i < additional.size()) { // We just need to not reach the end of the range
            result[i] = result[i] | additional[i];
        }
    }
}

void intersection(std::vector<bool>& result, const std::vector<bool>& additional){ // Intersection of 2 sets, result of operation contain in the first set
    int min_size = min_of_array_size(result.size(), additional.size());
    result.resize(min_size, false);
    for(int i=0; i < min_size; i++){
        result[i] = result[i] & additional[i];
    }
}

std::vector<bool> array_to_bit_array(const std::vector<int>& array) { // Reform default array to array of bytes
    int max_value = max_element_of_array(array);
    std::vector<bool> bit_array(max_value + 1, false); // Initialize bit array with false values
    for (int value : array) {
        if (value >= 0 && value <= max_value) {
            bit_array[value] = true;
        }
    }
    return bit_array;
}

void print_bit_array(const std::vector<bool>& bit_array) {
    for (bool bit : bit_array) {
        std::cout << (bit ? '1' : '0');
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> A, B, C, D, E;
    std::vector<bool> bA, bB, bC, bD, bE;
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
    // Algorythm
    bE = bA;
    association(bE, bB);
    intersection(bC, bD);
    association(bE, bC);
    
    for(int i=0; i<bE.size();i++){
        if(bE[i]==1){std::cout << i << " ";}
    }
    return 0;
}