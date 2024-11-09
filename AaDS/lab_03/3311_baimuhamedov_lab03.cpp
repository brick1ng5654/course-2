#include <iostream>
#include <string>

struct TreeNode { // structure of tree
    char symbol;
    int value; // value of int data
    TreeNode* left; // left child
    TreeNode* right; // right child
    std::string name;

    TreeNode(int val, const std::string&treeName) : value(val), left(nullptr), right(nullptr), name(treeName) {}
};

static char currentLetter = 'A';

std::string getNextTreeName() {
    if (currentLetter > 'Z') {
        throw std::runtime_error("Exceeded the limit of 26 trees.");
    }
    return std::string(1, currentLetter++);
}

// Функция для добавления узла в дерево
TreeNode* insert(TreeNode* root, int value) {
    if (root == nullptr) {
        std::string treeName = getNextTreeName();
        return new TreeNode(value, treeName);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

// Функция для печати дерева (in-order traversal)
void printInOrder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    printInOrder(root->left);
    std::cout << root->name << ": " << root->value << " ";
    printInOrder(root->right);
}

int main() {
    TreeNode* root = nullptr;

    // Добавление узлов в дерево
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    // Печать дерева
    std::cout << "In-order traversal of the tree: ";
    printInOrder(root);
    std::cout << std::endl;

    return 0;
}