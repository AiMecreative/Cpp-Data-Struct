#pragma once

#include <deque>
#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>
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
    int nodeCount;
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
    void write(const std::string &loc, int file_base, int size, int &p_file);

    T getMax();                         // return the max elem
    T getMin();                         // return the min elem
    int getNodeCount();                 // return the node number
    int getElemCount();                 // return the elem number
};

template<typename T>
PriorityDeque<T>::PriorityDeque() {
    elemCount = 0;
    nodeCount = 0;
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
void PriorityDeque<T>::push(T elem) {
    if (elemCount % 2 == 0) {
        node<T> newNode;
        int init = 0;
        newNode.right = *(T *) &init;
        newNode.left = elem;
        Deque.push_back(newNode);
        elemCount += 1;

        if (Deque.size() == 1) {
            return;
        } else {
            // elemCount >= 2
            int child;
            int parent;
            for (child = Deque.size(), parent = (Deque.size() - 1 - (Deque.size() % 2)) / 2;
                 parent >= 0;
                 child = parent, parent = (parent - ((parent + 1) % 2)) / 2) {
                if (Deque.at(child).left > Deque.at(child).right) {
                    std::swap(Deque.at(child).left, Deque.at(child).right);
                }
                if (Deque.at(child).left < Deque.at(parent).left) {
                    std::swap(Deque.at(child).left, Deque.at(parent).left);
                }
            }
        }
    } else {
        // elemCount % 2 == 1
        Deque.back().right = elem;
        if (Deque.back().left > Deque.back().right) {
            std::swap(Deque.back().left, Deque.back().right);
        }
        elemCount += 1;
        if (Deque.size() == 1) {
            return;
        } else {
            int child;
            int parent;
            for (child = Deque.size(), parent = (Deque.size() - 1 - (Deque.size() % 2)) / 2;
                 parent >= 0;
                 child = parent, parent = (parent - ((parent + 1) % 2)) / 2) {
                if (Deque.at(child).left > Deque.at(child).right) {
                    std::swap(Deque.at(child).left, Deque.at(child).right);
                }
                if (Deque.at(child).right > Deque.at(parent).right) {
                    std::swap(Deque.at(child).right, Deque.at(parent).right);
                }
            }
        }
    }
}

template<typename T>
void PriorityDeque<T>::popMax() {

}

template<typename T>
void PriorityDeque<T>::popMin() {

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
int PriorityDeque<T>::getNodeCount() {
    return nodeCount;
}

template<typename T>
void PriorityDeque<T>::write(const std::string &loc, int file_base, int size, int &p_file) {

}
