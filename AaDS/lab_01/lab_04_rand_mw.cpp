#include <iostream>
#include <vector>
#include <time.h>
#define REPEAT 10000000

void association(unsigned short &result, unsigned short additional){ // Association of 2 sets, result of operation contain in the first set
    result |= additional; // byte by byte association
}

void intersection(unsigned short &result, unsigned short additional){ // Intersection of 2 sets, result of operation contain in the first set
    result &= additional; // byte by bye intersection
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
    // Generate radnom machine words
    srand(time(0));
    int t1 = clock();
    for(int i=0; i<REPEAT; i++){
        unsigned short wA = rand()%0x3FF; // use short cause it had 16 bytes
        unsigned short wB = rand()%0x3FF; //0x3FF to range [0, 9]
        unsigned short wC = rand()%0x3FF;
        unsigned short wD = rand()%0x3FF;
        unsigned short wE = 0;
        // Algorithm
        wE = wA;
        association(wE,wB);
        intersection(wC, wD);
        association(wE, wC);
    }
    int t2 = clock();
    double t = double(t2-t1) / CLOCKS_PER_SEC;
    std::cout << "Time: " << t;
    
    return 0;
}