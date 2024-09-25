#include <vector>
#include <string>

class Set {
private:
    std::vector<int> set; // set realizied as array(vector)
    std::string name;
    int tag;
    static int tag_counter;

public:
    Set();
    ~Set();
    int size() const; // size = count of elem in set
    void set_the_name(std::string name);
    void input_in_set();
    void print_set() const;
    bool is_in_set(int element) const;
    void association(const Set& additional);
    void intersection(const Set& additional);
    void fill_random();
    void timer();
};