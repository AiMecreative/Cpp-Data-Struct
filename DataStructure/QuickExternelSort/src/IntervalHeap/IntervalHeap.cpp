#include "../defs.h"
#include "IntervalHeap.h"
#include <algorithm>
#include <math.h>

template <typename T>
IntervalHeap<T>::IntervalHeap()
{
    Head = 0;
    leftNum = 0;
    rightNum = 0;
    accssTimes = 0;
    elemNum = 0;

    HeapArray = new HeapNode<T>[MIDDLE_SIZE];
    for (int i = 0; i < MIDDLE_SIZE; i++)
    {
        HeapArray[i]->Left = (T)0;
        HeapArray[i]->Right = (T)0;
        HeapArray[i]->rightNull = true;
        HeapArray[i]->leftNull = true;
    }
}

template <typename T>
IntervalHeap<T>::~IntervalHeap() {}

template <typename T>
void IntervalHeap<T>::insert(T elem, Bundle<T> *buf)
{
    // detect whether the middle memory is full
    // not full
    if (max(leftNum, rightNum) < MIDDLE_SIZE)
    {
        elemNum += 1;
        if (elemNum % 2 == 0)
        {
            leftNum += 1;
            HeapArray[leftNum - 1]->Left = elem;
            HeapArray[leftNum - 1]->leftNull = false;
        }
        else // elemNum % 2 == 1
        {
            rightNum += 1;
            HeapArray[rightNum - 1]->Right = elem;
            HeapArray[rightNum - 1]->rightNull = false;
        }
    }
    else
    {
        int flag = accssTimes % 2;
        if (flag == 0)
        {
            buf->type = "min";
            buf->value = getMin();
        }
        else // flag == 1
        {
            buf->type = "max";
            buf->value = getMax();
        }
    }
}

template <typename T>
void IntervalHeap<T>::nodeInnerSwap(int idx)
{
    if (elemNum > 1)
    {
        if (leftNum < rightNum)
        {
            swap(HeapArray[rightNum - 1]->Right, HeapArray[rightNum - 1]->Left);
            rightNum -= 1;
            leftNum += 1;
        }
        else if (HeapArray[idx]->Right < HeapArray[idx]->Left)
            swap(HeapArray[idx]->Right, HeapArray[idx]->Left);
    }
}

template <typename T>
int IntervalHeap<T>::reconstruct(int parent_idx)
{
    if (elemNum == 0)
    {
        throw "EMPTY HEAP CAN'T BE RECONSTRUCTED";
        return parent_idx;
    }
    else if (elemNum == 1)
    {
        if (leftNum == 0 && rightNum == 1)
        {
            leftNum = 1;
            rightNum = 0;
            swap(HeapArray[Head]->Left, HeapArray[Head]->Right);
        }
        return parent_idx;
    }
    else if (elemNum == 2)
    {
        nodeInnerSwap(parent_idx);
        return parent_idx;
    }
    else // elemNum > 2
    {
        int left = parent_idx * 2 + 1;
        int right = parent_idx * 2 + 2;
        if (HeapArray[parent_idx]->Left > HeapArray[parent_idx]->Right)
            swap(HeapArray[parent_idx]->Left, HeapArray[parent_idx->Right]);
        if (left > leftNum - 1 || right > rightNum - 1) // leaf nodes
            return parent_idx;
        if (HeapArray[parent_idx]->Left > HeapArray[left]->Left)
        {
            reconstruct(left);
            swap(HeapArray[parent_idx]->Left, HeapArray[left]->Left);
            nodeInnerSwap(parent_idx);
        }
        if (HeapArray[parent_idx]->Right < HeapArray[right]->Right)
        {
            reconstruct(right);
            swap(HeapArray[parent_idx]->Right, HeapArray[right]->Right);
            nodeInnerSwap(parent_idx);
        }
    }
}

template <typename T>
T IntervalHeap<T>::getMin()
{
    T minElem = (T)0;
    if (elemNum == 0)
        throw "EMPTY HEAP HAS NO MINIMUM VALUE";
    else if (elemNum == 1)
    {
        minElem = HeapArray[Head]->Left;
        accssTimes += 1;
    }
    else if (elemNum == 2)
    {
        minElem = min(HeapArray[Head]->Left, HeapArray[Head]->Right);
        accssTimes += 1;
    }
    else // elemNum > 2
    {
        T temp = (T)0;
        minElem = HeapArray[Head]->Left;
        accssTimes += 1;
        if (rightNum == leftNum)
        {
            temp = HeapArray[leftNum]->Right;
            rightNum -= 1;
        }
        else // leftNum > rightNum
        {
            temp = HeapArray[leftNum]->Left;
            leftNum -= 1;
        }
        HeapArray[Head]->Left = temp;
        reconstruct(Head);
    }
    return minElem;
}

template <typename T>
T IntervalHeap<T>::getMax()
{
    T maxElem = (T)0;
    if (elemNum == 0)
        throw "EMPTY HEAP HAS NO MINIMUM VALUE";
    else if (elemNum == 1)
    {
        maxElem = HeapArray[Head]->Right;
        accssTimes += 1;
    }
    else if (elemNum == 2)
    {
        maxElem = max(HeapArray[Head]->Left, HeapArray[Head]->Right);
        accssTimes += 1;
    }
    else // elemNum > 2
    {
        T temp = (T)0;
        maxElem = HeapArray[Head]->Right;
        accssTimes += 1;
        if (rightNum == leftNum)
        {
            temp = HeapArray[leftNum]->Right;
            rightNum -= 1;
        }
        else // leftNum > rightNum
        {
            temp = HeapArray[leftNum]->Left;
            leftNum -= 1;
        }
        HeapArray[Head]->Right = temp;
        reconstruct(Head);
    }
    return maxElem;
}

template <typename T>
void IntervalHeap<T>::createHeap(T *elemList)
{
    for (auto elem: elemList)
        insert(elem);
    reconstruct(Head);
}

template<typename T>
void IntervalHeap<T>::print()
{
    if (elemNum == 0)
    {
        throw "NO OBJECTIVES FOUND IN INTERVALHEAP WHEN PRINTING";
        return;
    }
    for (int i = 0; i < MIDDLE_SIZE; i++)
    {
        printf(HeapArray[i]->Left, HeapArray[i]->Right);
    }
}