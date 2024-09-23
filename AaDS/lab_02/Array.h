#include <vector>
#include <string>

class Set {
private:
    std::vector<int> set;
    std::string name;

public:
    int size() const;
    void set_the_name(std::string name);
    void input_in_set();
    void print_set() const;
    bool is_in_set(int element) const;
    void association(const Set& additional);
    void intersection(const Set& additional);
    void fill_random();
    void timer();
};