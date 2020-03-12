#pragma once
#include <iostream>
#include "arraylist.h"

template<typename T>
class LinkedQueue : public ChainList<T>
{
public:
    LinkedQueue() : ChainList<T>() {}
    ~LinkedQueue() {}

    T front() const {
        checkStack();
        return this->get(this->size() - 1);
    }
    T back() const {
        checkStack();
        return this->get(0);
    }

    T pop() {
        checkStack();
        T value = this->get(this->size() - 1); 
        this->erase(this->size() - 1);
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
        if (this->empty()) throw "The queue is empty()";
    }
};
