#include "PriorityDeque.h"
#include <stdexcept>
#include <algorithm>
#include "defs.h"

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
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    return Deque.back().leftNull;
}

template<typename T>
bool PriorityDeque<T>::backRightNull() {
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    return Deque.back().rightNull;
}

template<typename T>
void PriorityDeque<T>::push(T elem) {
    // elemCount % 2 == 0
    if (!(elemCount % 2)) {
        QNode<T> interval;
        nodeCount += 1;
        // judge min-heap or max-heap
        if (elem > Deque.at((nodeCount - 1) / 2).Right) {
            interval.Right = elem;
            interval.Left = 0;
            interval.leftNull = true;
            interval.rightNull = false;
        } else {  // default insertion -- left
            interval.Left = elem;
            interval.Right = 0;
            interval.leftNull = false;
            interval.rightNull = true;
        }
        Deque.push_back(interval);
    } else {
        // elemCount % 2 == 1
        if (!Deque.back().leftNull)
            Deque.back().Left = elem;
        else
            Deque.back().Right = elem;
    }
    //TODO: 实现插入节点后, 用循环调整interval heap
}

template<typename T>
void PriorityDeque<T>::popMax() {
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    elemCount -= 1;
    if (elemCount == 1) {
        nodeCount -= 1;
        Deque.pop_front(); // now queue is empty
    } else if (elemCount == 2) {
        Deque.front().Right = 0;
        Deque.front().rightNull = true;  // now the queue is not empty, still one node left
    } else {
        // elemCount > 2
        Deque.front().Right = 0;
        Deque.front().rightNull = true;
        //TODO: 实现删除最大堆头节点后的调整
    }
}

template<typename T>
void PriorityDeque<T>::popMin() {
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    elemCount -= 1;
    if (elemCount == 1) {
        nodeCount -= 1;
        Deque.pop_front(); // now queue is empty
    } else if (elemCount == 2) {
        Deque.front().Left = Deque.front().Right;
        Deque.front().Right = 0;
        Deque.front().rightNull = true;
        Deque.front().leftNull = false;  // now the queue is not empty, still one node left
    } else {
        // elemCount > 2
        Deque.front().Left = 0;
        Deque.front().leftNull = true;
        //TODO: 实现删除最小堆头节点后的调整
    }
}

template<typename T>
T PriorityDeque<T>::getMax() {
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    if (elemCount == 1) {
        if (!Deque.front().leftNull) return Deque.front().Left;
        if (!Deque.front().rightNull) return Deque.front().Right;
    } else
        return Deque.front().Right;
}

template<typename T>
T PriorityDeque<T>::getMin() {
    if (Deque.empty())
        throw std::runtime_error("empty queue!");
    if (elemCount == 1) {
        if (!Deque.front().leftNull) return Deque.front().Left;
        if (!Deque.front().rightNull) return Deque.front().Right;
    } else
        return Deque.front().Left;
}

template<typename T>
int PriorityDeque<T>::getElemCount() {
    return elemCount;
}

template<typename T>
int PriorityDeque<T>::getNodeCount() {
    return nodeCount;
}