#pragma once
#include <iostream>
#include "arraylist.h"

template<typename T>
class LinkedStack: public ChainList<T>
{
public:
    LinkedStack(): ChainList<T>() {}
    ~LinkedStack(){}

    T top() const {
        checkStack();
        return this->get(0);
    }
    T pop() {
        checkStack();
        T value = this->get(0); this->erase(0);
        return value;
    }
    void push(const T& value) {
        this->insert(0, value);
    }

    void print() {
        for (int i = 0; i < this->size(); i++)
            std::cout << this->get(i) << " ";
        std::cout << std::endl;
    }

private:
    void checkStack() const {
        if (this->empty()) throw "The stack is empty()";}
};
