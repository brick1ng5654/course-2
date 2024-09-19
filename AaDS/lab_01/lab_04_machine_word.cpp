#include <iostream>
#include <vector>
#include <sstream>
#include <string>

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

void association(unsigned short &result, unsigned short additional){ // Association of 2 sets, result of operation contain in the first set
    result |= additional; // byte by byte association
}

void intersection(unsigned short &result, unsigned short additional){ // Intersection of 2 sets, result of operation contain in the first set
    result &= additional; // byte by byte intersection
}

void array_to_machine_word(unsigned short &number ,const std::vector<int>& array){
    for(int i=0; i<sizeof(unsigned short)*8; i++){
        if(is_in_set(i, array)){
            number |= (1 << i); // to bytes of number add 1 if i in array
        }
    }
}

void print_machine_word(unsigned short &number){
    for(int i=0; i<10; i++){
        if(number & (1 << i)){
            std::cout << i << " ";
        }
    }
    std::cout << "\n\t\t";
    for(int i=sizeof(number)*8-1; i>=0; i--){
        if(number & (1 << i)){
            std::cout << 1;
        } else std::cout << 0;
    }
    std::cout <<std::endl;
}

int main() {
    std::vector<int> A, B, C, D, E;
    unsigned short wA = 0;
    unsigned short wB = 0;
    unsigned short wC = 0;
    unsigned short wD = 0;
    unsigned short wE;
    // Enter the data
    input_in_vector(A, "A"); // 1, 2
    input_in_vector(B, "B"); // 4, 7
    input_in_vector(C, "C"); // 3, 5
    input_in_vector(D, "D"); // 5, 8, 9
    // Reform to machine words
    array_to_machine_word(wA, A);
    array_to_machine_word(wB, B);
    array_to_machine_word(wC, C);
    array_to_machine_word(wD, D);
    // Algorithm
    wE = wA;
    association(wE,wB);
    intersection(wC, wD);
    association(wE, wC);
    print_machine_word(wE);
    return 0;
}