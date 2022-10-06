#pragma once

#include <deque>
#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <cassert>
#include "defs.h"

template<typename T>
struct node {
    T left;
    T right;
};

template<typename T>
class PriorityDeque {
private:
    int elemCount;
public:
    std::deque<node<T> > Deque;

    PriorityDeque();

    ~PriorityDeque();

    bool isEmpty();                     // judge the priority deque is empty
    bool backRightNull();               // if the last node have right element
    bool backLeftNull();                // if the last node have left element
    void push(T elem);                  // insert elem in back node
    void popMax();                      // delete the max elem and adjust the structure
    void popMin();                      // delete the min elem and adjust the structure

    void adjust();

    void write(const std::string &loc, int file_base, int size, int &p_file);

    T getMax();                         // return the max elem
    T getMin();                         // return the min elem
    int getElemCount();                 // return the elem number
};

template<typename T>
PriorityDeque<T>::PriorityDeque() {
    elemCount = 0;
}

template<typename T>
PriorityDeque<T>::~PriorityDeque() = default;

template<typename T>
bool PriorityDeque<T>::isEmpty() {
    return Deque.empty();
}

template<typename T>
bool PriorityDeque<T>::backLeftNull() {
    if (elemCount % 2 == 0) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool PriorityDeque<T>::backRightNull() {
    if (elemCount % 2 == 1) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
void PriorityDeque<T>::adjust() {
    if (elemCount == 0) {
        return;
    } else if (elemCount % 2 == 0) {
        // elemCount >= 2, even
        int child;
        int parent;
        for (child = Deque.size(), parent = (Deque.size() - 1 - (Deque.size() % 2)) / 2;
             parent >= 0;
             child = parent, parent = (parent - ((parent + 1) % 2)) / 2) {
            if (Deque.at(child).left > Deque.at(child).right && child != Deque.size() - 1) {
                std::swap(Deque.at(child).left, Deque.at(child).right);
            }
            if (Deque.at(child).left < Deque.at(parent).left) {
                std::swap(Deque.at(child).left, Deque.at(parent).left);
            } else {
                break;
            }
        }
    } else {
        // elemCount % 2 == 1
        int child;
        int parent;
        for (child = Deque.size(), parent = (Deque.size() - 1 - (Deque.size() % 2)) / 2;
             parent >= 0;
             child = parent, parent = (parent - ((parent + 1) % 2)) / 2) {
            if (Deque.at(child).left > Deque.at(child).right && child != Deque.size() - 1) {
                std::swap(Deque.at(child).left, Deque.at(child).right);
            }
            if (Deque.at(child).right > Deque.at(parent).right) {
                std::swap(Deque.at(child).right, Deque.at(parent).right);
            } else {
                break;
            }
        }
    }
}

template<typename T>
void PriorityDeque<T>::push(T elem) {
    if (elemCount % 2 == 0) {
        node<T> newNode;
        int init = 0;
        newNode.right = *(T *) &init;
        newNode.left = elem;
        Deque.push_back(newNode);
        elemCount += 1;
    } else {
        // elemCount % 2 == 1
        Deque.back().right = elem;
        if (Deque.back().left > Deque.back().right) {
            std::swap(Deque.back().left, Deque.back().right);
        }
        elemCount += 1;
    }
    adjust();
}

template<typename T>
void PriorityDeque<T>::popMax() {
    if (Deque.empty()) {
        throw std::runtime_error("empty Deque can't popMax");
    }
    if (Deque.size() == 1 && elemCount == 1) {
        Deque.pop_front();
        elemCount -= 1;
    } else if (Deque.size() == 1 && elemCount == 2) {
        int init = 0;
        Deque.front().right = *(T *) &init;
        elemCount -= 1;
    } else {
        if (elemCount % 2 == 0) {
            Deque.front().right = Deque.back().right;
        } else {
            Deque.front().right = Deque.at(Deque.size() - 2).right;
            Deque.at(Deque.size() - 2).right = Deque.back().left;
            Deque.pop_back();
        }
        elemCount -= 1;
        adjust();
    }
}

template<typename T>
void PriorityDeque<T>::popMin() {
    int init = 0;
    if (Deque.empty()) {
        throw std::runtime_error("empty Deque can't popMax");
    }
    if (Deque.size() == 1 && elemCount == 1) {
        Deque.pop_front();
        elemCount -= 1;
    } else if (Deque.size() == 1 && elemCount == 2) {
        Deque.front().left = Deque.front().right;
        Deque.front().right = *(T *) &init;
        elemCount -= 1;
    } else {
        if (elemCount % 2 == 0) {
            Deque.front().left = Deque.back().left;
            Deque.back().left = Deque.back().right;
            Deque.back().right = *(T *) &init;
        } else {
            Deque.front().left = Deque.back().left;
            Deque.pop_back();
        }
        elemCount -= 1;
        adjust();
    }
}

template<typename T>
T PriorityDeque<T>::getMax() {
    if (Deque.empty()) {
        throw std::runtime_error("empty Deque can't getMax!");
    }
    return Deque.front().right;
}

template<typename T>
T PriorityDeque<T>::getMin() {
    if (Deque.empty()) {
        throw std::runtime_error("empty Deque can't getMin");
    }
    return Deque.front().left;
}

template<typename T>
int PriorityDeque<T>::getElemCount() {
    return elemCount;
}

template<typename T>
void PriorityDeque<T>::write(const std::string &loc, int file_base, int size, int &p_file) {
    if (Deque.empty()) {
        return;
    }
    std::ifstream write_file{loc};
    assert(write_file.is_open() && write_file.good());
    write_file.seekg(file_base, std::ios::beg);
    while (!Deque.empty()) {
        T value = getMin();
        popMin();
        write_file << value;
    }
    p_file = write_file.tellg() / sizeof(T) - 1;
    write_file.close();
}
