#include <iostream>
#include <queue>
#include <string>
#include <iomanip>

struct TreeNode {
    std::string name;
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr), name("") {}
};

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

void insert(TreeNode*& root, const std::string& input){
    if(input.empty()) return;

    std::queue<TreeNode*> q;
    root = new TreeNode(input[0] - '0');
    q.push(root);

    size_t i = 1;
    while(i<input.length()){
        TreeNode *current = q.front();
        q.pop();

        if(input[i] == '1'){
            current->left = new TreeNode(input[i] - '0');
            q.push(current->left);
        }
        i++;

        if(i<input.length() && input[i]=='1'){
            current->right = new TreeNode(input[i] - '0');
            q.push(current->right);
        }
        i++;
    }
}

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

int main(){
    std::string input_string;
    TreeNode* root = nullptr;

    std::cout << "Enter the string: ";
    std::cin >> input_string;
    insert(root, input_string);
    assign_names_by_level(root);

    print_tree(root);
    // std::cout << root->value;
    return 0;
}