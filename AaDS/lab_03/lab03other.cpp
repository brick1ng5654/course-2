#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <iomanip>
using namespace std;

const int scale = 256;

class Node {
    char d;     // тег узла
    Node *lft;  // левый сын
    Node *rgt;  // правый сын
    public:

    Node() : lft(nullptr), rgt(nullptr) {} // конструктор узла
    ~Node() {
        if (lft) delete lft; // деструктор (уничтожает поддерево)
        if (rgt) delete rgt;
    }
    friend class Tree;  // дружественный класс «дерево»

};

class Tree {
    Node *root;    // указатель на корень дерева
    char num, maxnum;       // счётчик тегов и максимальный тег
    int maxrow;     // максимальная глубина
    Node* MakeNode(const string& input, size_t& index, int depth);  
    void printTree(Node* node, string indent, bool isLast, char nodeType); // красивый вывод поддерева
public:
    Tree(char num, char maxnum, int maxrow);
    ~Tree();
    void fill_levels(Node* node, vector<vector<string>>& levels, int depth, int pos, int width);
    void MakeTree(const string& input);
    bool exist() { return root != nullptr; } // проверка «дерево не пусто»
    Node* getRoot() { return root; } // геттер для корня
    void OutTree(); // выдача на экран
};

Tree::Tree(char nm, char mnm, int mxr) : 
      num(nm), maxnum(mnm), maxrow(mxr), root(nullptr) {}

Tree::~Tree() {
    delete root;
}

Node* Tree::MakeNode(const string& input, size_t& index, int depth) {
    if (index >= input.length() || input[index] == '0') return nullptr; // Проверка на максимальную глубину
    Node* v = nullptr;
    if (num <= maxnum) {
        v = new Node;
        v->d = num++;
        index++;
        if (index < input.length() && input[index] != '0') {
            v->lft = MakeNode(input, index, depth + 1);
        }
        if (index < input.length() && input[index] == '0') {
            index++; // Пропускаем '0' и возвращаемся на уровень выше
        }
        if (index < input.length() && input[index] != '0') {
            v->rgt = MakeNode(input, index, depth + 1);
        }
    }
    return v;
}

void Tree::MakeTree(const string& input) {
    size_t index = 0;
    root = MakeNode(input, index, 0);
}

void Tree::fill_levels(Node* node, vector<vector<string>>& levels, int depth, int pos, int width){
    if(node == nullptr) return;

    levels[depth][pos] = string(1, node->d);

    int offset = width/2;
    if(offset>0){
        fill_levels(node->lft, levels, depth+1, pos-offset, offset);
        fill_levels(node->rgt, levels, depth+1, pos+offset, offset);
    }
}

void Tree::OutTree() {
    int depth = maxrow;
    int width = (1<<depth)-1;
    vector<vector<string>> levels(depth, vector<string>(width, "-"));

    fill_levels(root, levels, 0, (width-1)/2, (width-1)/2);

    for(int i =depth-1; i>=0; i--){
        for(int j=0;j<width;j++){
            cout << setw(1) << levels[i][j];
        }
        cout << endl << endl;
    }
}

template <class Item> class QUEUE {
    Item * Q; int h, t, N;
public:
    QUEUE(int maxQ) : h(0), t(0), N(maxQ), Q(new Item[maxQ + 1]) { }
    int empty() const { return (h % N) == t; }
    void push(Item item) { Q[t++] = item; t %= N; }
    Item pop() { h %= N; return Q[h++]; }
};

int main(){
    // cout << "Enter tree manually:";
    string input_string;
    Tree Tr('A', 'Z', 7);
    srand(time(nullptr));
    cout << "Enter the string: ";
    cin >> input_string;
    cout << "\n";
    Tr.MakeTree(input_string);
    if (Tr.exist()) {
        // output
        Tr.OutTree();
    } else cout << "Tree doesn't exist";
    return 0;
}
