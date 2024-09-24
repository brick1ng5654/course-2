// #include "Array.h"
// #include "List.h"
// #include "Barray.h"
#include "Mword.h"
#include <vector>
#include <string>
#include <time.h>

// g++ main.cpp Array/Lists/Barray/Mword.cpp -o lab02

int main() {
    srand(time(0));
    std::vector<Set> sets(5);
    
    for(int i=0; i<4;i++){
        sets[i].set_the_name("Set "+std::to_string(i+1));
        sets[i].fill_random();
        sets[i].print_set();
    }
    sets[4] = sets[0];
    sets[4].association(sets[1]);
    sets[2].intersection(sets[3]);
    sets[4].association(sets[2]);

    sets[4].set_the_name("Resulting set");
    sets[4].print_set();
    sets[4].timer();
    return 0;
}