#include <iostream>
#include <string>
#include <time.h>
#include <random>
#include <cmath>
#include <iomanip>
#include <queue>


struct TreeNode { // structure of tree
    int value; // value of int data
    TreeNode* left; // left child
    TreeNode* right; // right child
    std::string name;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr), name("") {}
};

void fill_levels(TreeNode* node, std::vector<std::vector<std::string>>& levels, int depth, int pos, int width) {
    if (node == nullptr) return;

    // levels[depth][pos] = std::to_string(node->value); // добавляем значение узла
    levels[depth][pos] = node->name;

    int offset = width / 2; // Делим ширину пополам для следующего уровня
    if (offset > 0) { 
        // Рекурсивно добавляем дочерние узлы, сдвигая на половину оставшейся ширины
        fill_levels(node->left, levels, depth + 1, pos - offset, offset);
        fill_levels(node->right, levels, depth + 1, pos + offset, offset);
    }
}

void print_tree(TreeNode* root) {
    int depth = 5; // Максимальная глубина (количество уровней)
    int width = (1 << depth) - 1; // Ширина для центрирования дерева

    std::vector<std::vector<std::string>> levels(depth, std::vector<std::string>(width, "-")); // Создаем пустую сетку

    fill_levels(root, levels, 0, (width - 1) / 2, (width - 1) / 2); // Начинаем с ширины всего уровня

    // Печатаем уровни

    for(int i=depth-1; i>=0; i--){
        for(int j=0; j<width; j++){
                std::cout << std::setw(3) << levels[i][j];
            }
        std::cout << std::endl << std::endl;

    }
}

TreeNode* insert(TreeNode* root, int value) {
    if (root == nullptr) {
        return new TreeNode(value);
    }

    if (value <= root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

TreeNode* create_tree(TreeNode* root, int node_count){
    int root_value;
    root_value = rand()%100;
    root = new TreeNode(root_value);
    for(int i=0; i<node_count;i++){
        int value = rand()%100;
        root = insert(root, value);
    }
    return root;
}

// Функция для поуровневого присвоения имен узлам
void assign_names_by_level(TreeNode* root) {
    if (root == nullptr) return;

    std::queue<TreeNode*> q;
    q.push(root);
    char current_name = 'A';

    // Обходим дерево по уровням
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();

        node->name = std::string(1, current_name++); // присваиваем текущий символ и инкрементируем

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

// Рекурсивная функция для вычисления высоты дерева
int calculate_height(TreeNode* node) {
    if (node == nullptr) return 0;

    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);

    return 1 + std::max(left_height, right_height);
}

// Функция для вычисления высоты правого поддерева корня
int height_of_right_subtree(TreeNode* root) {
    if (root == nullptr || root->right == nullptr) return 0;

    return calculate_height(root->right);
}

int main(){
    srand(time(0));
    int node_count = 5 + rand() % 10;
    TreeNode* root = nullptr;
    root = create_tree(root ,node_count);
    assign_names_by_level(root);
    print_tree(root);
    int height_of_r_subtree = height_of_right_subtree(root);
    std::cout << "Height of right subtree: " << height_of_r_subtree << "\n" << std::endl;
    return 0;
}