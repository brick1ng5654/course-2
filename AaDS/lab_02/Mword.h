#include <vector>
#include <string>

class Set {
private:
    unsigned short set;
    std::string name;

public:
    void set_the_name(std::string name);
    void print_set() const;
    void association(const Set& additional);
    void intersection(const Set& additional);
    void fill_random();
    void timer();
};