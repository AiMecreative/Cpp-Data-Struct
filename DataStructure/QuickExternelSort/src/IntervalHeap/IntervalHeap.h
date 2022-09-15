/*
intro: use min-max-heap to find the max/min value
left: min heap
right: max heap
 */
#pragma once
template <typename T>
struct HeapNode
{
    T Left;
    T Right;
    bool rightNull;
    bool leftNull;
};

template <typename T>
class IntervalHeap
{
private:
    int Head;
    int leftNum;
    int rightNum;
    int accssTimes; // add 1 when invoking get_ functions
    int elemNum;
    HeapNode<T> *HeapArray;

public:
    IntervalHeap();
    ~IntervalHeap();
    T getMin();
    T getMax();
    bool isEmpty();
    int reconstruct(int parent_idx);
    void print();
    void createHeap(T *elemList);
    void insert(T elem, Bundle<T> *buf);
    void nodeInnerSwap(int idx);
};
