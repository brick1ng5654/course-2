#include "Array.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <limits>

int Set::size() const{
    return set.size();
}

void Set::set_the_name(std::string name){
    this->name = name;
}

void Set::input_in_set(){
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
    std::cout<<"Enter the numbers of " << name << " separated by space: ";
    std::getline(std::cin, input);

    std::stringstream ss(input);
    int number;

    while(ss >> number){
        set.push_back(int(number));
    }
}

void Set::print_set() const{
    std::cout << name << ": ";
    for(int i=0; i<set.size();i++){
        std::cout << set[i] << " ";
    }
    std::cout << std::endl;
}

bool Set::is_in_set(int element) const {
    for (int i = 0; i < set.size(); i++) {
        if (element == set[i]) return true;
    }
    return false;
}

void Set::association(const Set& additional) {
    for (int i = 0; i < additional.size(); i++) {
        if (!is_in_set(additional.set[i])) {
            set.push_back(additional.set[i]);
        }
    }
}

void Set::intersection(const Set& additional) {
    auto number = set.begin();
    while (number != set.end()) {
        if (!additional.is_in_set(*number)) {
            number = set.erase(number); // Delete the value of set
        } else {
            number++;
        }
    }
}

void Set::fill_random(){
    for (int i = 0; i < (rand() % 10); i++) {
        int num = rand() % 10;
        if(!is_in_set(num)) set.push_back(num);
    }
}

void Set::timer(){
    int t1 = clock();
    for(int i=0; i<100000;i++){
        std::vector<Set> sets(5);
        for(int i=0; i<4;i++){
            sets[i].set_the_name("Set "+std::to_string(i+1));
            sets[i].fill_random();
        }
        sets[4] = sets[0];
        sets[4].association(sets[1]);
        sets[2].intersection(sets[3]);
        sets[4].association(sets[2]);
    }
    int t2 = clock();
    double t = double(t2 - t1) / CLOCKS_PER_SEC;
    
    std::cout << "Time: " << t << "*10^(-5) sec";
}