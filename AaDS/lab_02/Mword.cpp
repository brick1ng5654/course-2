#include "Mword.h"
#include <iostream>
#include <vector>
#include <time.h>

void Set::set_the_name(const std::string name){
    this->name = name;
}

void Set::association(const Set& additional) {
    set |= additional.set; // Побитовое ИЛИ для объединения
}

void Set::intersection(const Set& additional) {
    set &= additional.set; // Побитовое И для пересечения
}

void Set::print_set() const{
    std::cout << name << ": ";
    for(int i=0; i<10; i++){
        if(set & (1 << i)){
            std::cout << i << " ";
        }
    }
    std::cout << "\n\t\t";
    for(int i=sizeof(set)*8-1; i>=0; i--){
        if(set & (1 << i)){
            std::cout << 1;
        } else std::cout << 0;
    }
    std::cout <<std::endl;
}

void Set::fill_random(){
    set = rand()%0x3FF;
}

void Set::timer(){
    srand(time(0)); // To random values
    int t1 = clock();
    for(int i=0; i<10000000; i++){
        Set wA, wB, wC, wD, wE;
        wA.fill_random();
        wB.fill_random();
        wC.fill_random();
        wD.fill_random();
        wE = wA;
        wE.association(wB);
        wC.intersection(wD);
        wE.association(wC);
    }
    int t2 = clock();
    double t = double(t2-t1) / CLOCKS_PER_SEC;
    std::cout << "Time: " << t << "*10^(-7) sec";
}