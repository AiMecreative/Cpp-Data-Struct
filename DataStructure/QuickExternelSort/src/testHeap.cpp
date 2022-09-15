/*
used for test IntervalHeap lib
execusion: ./testHeap -elemNum
 */
#include "./IntervalHeap/IntervalHeap.h"
#include <iostream>
#include <random>
#include <string>

using namespace std;

bool isNum(string str)
{
    for (auto &c : str)
        if (!isdigit(c))
            return false;
    return true;
}

int main(int argn, char **args)
{
    char *cmd;
    if (argn == 2)
        cmd = args[1];
    else
    {
        throw "COMMAND NOT FOUND";
        return 1;
    }
    if (!isNum(cmd))
    {
        throw "UNDEFINED COMMAND";
        return 1;
    }

    int elemNum = atoi(cmd);
    // initial template with INT
    int *elemList = new int[elemNum];
    for (int i = 0; i < elemNum; i++)
    {
        elemList[i] = rand() % 100;
        cout << elemList[i] << " ";
    }
    cout << endl;
    IntervalHeap<int> intervalHeap = IntervalHeap<int>();
    intervalHeap.createHeap(elemList);
    intervalHeap.print();
    return 0;
}