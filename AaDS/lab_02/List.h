#include <vector>
#include <string>

struct Node{ // Structure of Node
        int data;
        Node *next;
};

class Set {
private:
    Node *set = nullptr;
    std::string name;

public:
    Set();
    void delete_list();
    ~Set();
    void push_back(int value);
    void set_the_name(std::string name);
    void input_in_set();
    void print_set();
    bool is_in_set(int element);
    void association(const Set &additional);
    void intersection(Set& additional);
    void fill_random();
    void timer();
};