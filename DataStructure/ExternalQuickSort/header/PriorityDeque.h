#ifndef PRIORITYDEQUE_H
#define PRIORITYDEQUE_H

#include <deque>
#include <queue>
#include <vector>

template<typename T>
struct QNode {
    T Left;
    T Right;
    bool rightNull;
    bool leftNull;
};

template<typename T>
class PriorityDeque {
private:
    std::deque<QNode<T> > Deque;
    int elemCount;
    int nodeCount;
public:
    PriorityDeque();

    ~PriorityDeque();

    bool isEmpty();                     // judge the priority deque is empty
    bool backRightNull();               // if the last node have right element
    bool backLeftNull();                // if the last node have left element
    void push(T elem);                  // insert elem in back node
    void popMax();                      // delete the max elem and adjust the structure
    void popMin();                      // delete the min elem and adjust the structure
    T getMax();                         // return the max elem
    T getMin();                         // return the min elem
    int getNodeCount();                 // return the node number
    int getElemCount();                 // return the elem number
};

#endif //PRIORITYDEQUE_H
