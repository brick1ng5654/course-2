#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#define REPEAT 100000

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

void generate_random_numbers(std::vector<int> &array) {
    for (int i = 0; i < (rand() % 10); i++) {
        int num = rand() % 10;
        if(!is_in_set(num, array)){
            array.push_back(num);
        }
    }
}

void print_array(const std::vector<int> array){
    for(int i=0; i<array.size();i++){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    srand(time(0)); 
    // Enter the data
    int t1 = clock();
    for(int i=0; i<REPEAT;i++){
        std::vector<int> A, B, C, D, E;
        generate_random_numbers(A);
        generate_random_numbers(B);
        generate_random_numbers(C);
        generate_random_numbers(D);
        E = A;
        association(E, B);
        intersection(C, D);
        association(E, C);
    }
    int t2 = clock();
    double t = double(t2 - t1) / CLOCKS_PER_SEC;
    
    std::cout << "Time: " << t;
    
    return 0;
}