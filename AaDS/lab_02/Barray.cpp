#include "Barray.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <bitset>
#define SIZE 10

void Set::set_the_name(const std::string name){
    this->name = name;
}

void Set::association(const Set& additional) {
    set |= additional.set; // byte by byte OR
}

void Set::intersection(const Set& additional) {
    set &= additional.set; // byte by byte AND
}

void Set::print_set() const{
    std::cout << name << ": ";
    for(int i=0; i<10; i++){
        if(set[i]==1){
            std::cout << i << " ";
        }
    }
    std::cout << "\n\t\t";
    for(int i=9; i>=0; i--){
        if(set[i]==1) std::cout << 1;
        else std::cout << 0;
    }
    std::cout << std::endl;
}

void Set::fill_random(){
    set = std::bitset<SIZE> (rand() % 1024);
}

void Set::timer(){
    srand(time(0)); // To random values
    int t1 = clock();
    for(int i=0; i<10000000; i++){
        Set bA, bB, bC, bD, bE;
        bA.fill_random();
        bB.fill_random();
        bC.fill_random();
        bD.fill_random();
        bE = bA;
        bE.association(bB);
        bC.intersection(bD);
        bE.association(bC);
    }
    int t2 = clock();
    double t = double(t2-t1) / CLOCKS_PER_SEC;
    std::cout << "Time: " << t << "*10^(-7) sec";
}