#include "List.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <time.h>

Set::Set() : set(nullptr) {}

void Set::delete_list() {
    while (set) {
        Node* temp = set;
        set = set->next;
        delete temp;
    }
}

Set::~Set() {
    delete_list();
}

void Set::push_back(int value) { // Function of adding the last elem in list
    Node* newNode = new Node{ value, nullptr }; // Create new node
    if (!set) { // If list is empty
        set = newNode;
    }
    else { // If list is not empty
        Node* current = set;
        while (current->next) { // Find the last elem of list
            current = current->next;
        }
        current->next = newNode;
    }
}

void Set::set_the_name(std::string name){
    this->name = name;
}

void Set::print_set() {
    Node* current = set; // Создаем временный указатель, чтобы не изменить set
    std::cout << name << ": ";
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

bool Set::is_in_set(int element){
    Node* current = set; // Создаем временный указатель
    while (current) {
        if (element == current->data) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void Set::association(const Set additional) {
    Node* current = additional.set;
    while (current) {
        if (!is_in_set(current->data)) {
            push_back(current->data);
        }
        current = current->next;
    }
}

void Set::intersection(Set& additional) {
    Node* current = set;
    Node* prev = nullptr;
    while (current) {
        if (!additional.is_in_set(current->data)) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                set = current->next;
            }
            delete current;
            current = prev ? prev->next : set;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

void Set::fill_random() {
    int count = rand() % 10;
    while (count > 0) {
        int num = rand() % 10;
        if (!is_in_set(num)) {
            this->push_back(num);
            count--;
        }
    }
}

void Set::timer() {
    srand(time(0)); // Инициализация генератора случайных чисел

    int t1 = clock();
    for (int i = 0; i < 100000; i++) {
        Set A_list, B_list, C_list, D_list, CiD_list, E_list;

        A_list.fill_random();
        B_list.fill_random();
        C_list.fill_random();
        D_list.fill_random();

        E_list.association(A_list);
        E_list.association(B_list);
        CiD_list.intersection(C_list);
        E_list.association(CiD_list);
    }
    int t2 = clock();

    double t = double(t2 - t1) / CLOCKS_PER_SEC;
    std::cout << "Time: " << t << " sec" << std::endl;
}