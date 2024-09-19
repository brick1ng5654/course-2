#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// List

struct Node{ // Structure of Node
    int data;
    Node *next;
};

void push_back(Node*& head, int value){ // Function of adding the last elem in list
    Node* newNode = new Node{value, nullptr}; // Create new node
    if(!head){ // If list is empty
        head = newNode;
    } else { // If list is not empty
        Node* current = head;
        while(current->next){ // Find the last elem of list
            current = current->next;
        }
        current->next = newNode;
    }
}

void print_list(Node* head){ // Output list
    while(head){
        std::cout << head->data << " ";
        head=head->next;
    }
    std::cout << std::endl;
}

Node* array_to_list(const std::vector<int>& array){
    Node *list = nullptr;
    for(int number : array){
        push_back(list, number);
    }
    return list;
}

void input_in_vector(std::vector<int> &inputed, const std::string &name){
    std::string input;
    std::cout<<"Enter the numbers of " << name << " separated by space: ";
    std::getline(std::cin, input);

    std::stringstream ss(input);
    int number;

    while(ss >> number){
        inputed.push_back(int(number));
    }
}

bool is_in_list(int element, Node *list){
    while(list){
        if(element == list->data) return true;
        else list = list->next;
    }
    return false;
}

void association(Node *&res_list, Node *ad_list){
    while(ad_list){
        if(!is_in_list(ad_list->data, res_list)){push_back(res_list, ad_list->data);}
        ad_list = ad_list->next;
    }
}

void intersection(Node *&res_list,Node *ad1_list, Node *ad2_list){
    while(ad1_list){
        if(is_in_list(ad1_list->data, ad2_list)){
            push_back(res_list, ad1_list->data);
        }
        ad1_list = ad1_list->next;
    }
}

int main() {
    // Init
    std::vector<int> A, B, C, D, E;
    Node* A_list = nullptr;
    Node* B_list = nullptr;
    Node* C_list = nullptr;
    Node* D_list = nullptr;
    Node* CiD_list = nullptr;
    Node* E_list = nullptr;
    // Enter the data
    input_in_vector(A, "A"); // 1, 2
    input_in_vector(B, "B"); // 4, 7
    input_in_vector(C, "C"); // 3, 5
    input_in_vector(D, "D"); // 5, 8, 9
    // Reform to list
    A_list = array_to_list(A);
    B_list = array_to_list(B);
    C_list = array_to_list(C);
    D_list = array_to_list(D);
    //Algorithm
    association(E_list, A_list);
    association(E_list, B_list);
    intersection(CiD_list, C_list, D_list);
    association(E_list, CiD_list);

    print_list(E_list);
    
    return 0;
}