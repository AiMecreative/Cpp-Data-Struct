#ifndef EXTERNALQS_H
#define EXTERNALQS_H

#include "defs.h"
#include "PriorityDeque.h"
#include <vector>
#include <string>

template<typename T>
class ExternalQS {
private:
    std::vector<T> elemVec;
public:
    //TODO: 实现下面的函数
    ExternalQS();

    ~ExternalQS();

    void read_txt(std::string loc);      // read in data from txt
    void input();                        // load data into input buffer
    void to_small();                     // load data from middle memo to small memo
    void to_large();                     // load data from middle memo to large memo
    void write_small();                  // write into disk when small memo is full, before middle's loc
    void write_large();                  // write into disk when large memo is full, behind middle's loc
    void replace();                      // insert element into middle memo
    void sort();                         // sort recursively and store the elements into txt

    //TODO: 有时间就写
    void show(int elem_num);             // show the first elem_num elements of the result
};

#endif // EXTERNALQS_H