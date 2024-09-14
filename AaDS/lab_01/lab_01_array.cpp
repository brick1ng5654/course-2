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

void association(std::vector<int>& result, const std::vector<int>& additional){ // Association of 2 sets, result of operation contain in the first set
    for(int i=0;i<additional.size();i++){
        if(!is_in_set(additional[i], result)){result.push_back(additional[i]);}
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

int main() {
    std::vector<int> A, B, C, D, E;
    // Enter the data
    input_in_vector(A, "A"); // 1, 2
    input_in_vector(B, "B"); // 4, 7
    input_in_vector(C, "C"); // 3, 5
    input_in_vector(D, "D"); // 5, 8, 9
    // Algorythm
    E = A;
    association(E, B);
    intersection(C, D);
    association(E, C);
    // Output {1, 2, 4, 7, 5}
    std::cout << "Resulting set: ";
    for(int i = 0; i < E.size(); i++) {
        std::cout << E[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}