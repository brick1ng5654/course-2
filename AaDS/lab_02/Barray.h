#include <vector>
#include <string>
#include <bitset>
#define SIZE 10

class Set {
private:
    std::bitset<SIZE> set;
    std::string name;

public:
    void set_the_name(std::string name);
    void print_set() const;
    void association(const Set& additional);
    void intersection(const Set& additional);
    void fill_random();
    void timer();
};