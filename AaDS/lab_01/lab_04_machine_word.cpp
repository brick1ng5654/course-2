#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <bitset>

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

void association(unsigned int result, unsigned int additional){ // Association of 2 sets, result of operation contain in the first set
    for(int i=0; i<16; i++){
        if(additional & (1<<i)){
           result |= (1<<i); 
        }
    }
}

void intersection(std::vector<int>& result, const std::vector<int>& additional){ // Intersection of 2 sets, result of operation contain in the first set
    auto number = result.begin();
    while(number!=result.end()){
        if(!is_in_set(*number, additional)){
            number = result.erase(number); // Delete the value of set
        } else { number++;}
    }
}

void array_to_machine_word(unsigned int &number ,const std::vector<int>& array){
    for(int i=0; i<16; i++){
        if(is_in_set(i, array)){
            number |= (1 << i);
        }
    }
}

void print_machine_word(unsigned int &number){
    for(int i=0; i<16; i++){
        if(number & (1 << i)){
            std::cout << i << " ";
        }
    }
    std::cout <<std::endl;
}

int main() {
    std::vector<int> A, B, C, D, E;
    unsigned int wA, wB, wC, wD, wE;
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
    // Algorythm
    wE = wA;
    print_machine_word(wA);
    association(wE,wB);
    print_machine_word(wE);
    return 0;
}

