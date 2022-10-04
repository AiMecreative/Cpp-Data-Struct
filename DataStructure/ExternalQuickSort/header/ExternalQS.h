#pragma once

#include "defs.h"
#include "PriorityDeque.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <random>
#include <queue>

template<typename T>
class ExternalQS {
private:
    bool small_full;
    bool large_full;
    std::queue<T> input_buf;
    std::queue<T> small_buf;
    std::queue<T> large_buf;
public:
    PriorityDeque<T> *qs_queue; // middle
    const std::string loc;

    //TODO: 实现下面的函数
    ExternalQS() {
        qs_queue = new PriorityDeque<T>;
        small_full = false;
        large_full = false;
    }

    ~ExternalQS() = default;

    // generate rand number and write to txt
    void generate_rand(const std::string &type_name,
                       int bottom, int top, int count);

    // read in data from txt, `p_file` is the tail of file
    void read_to_mid(int file_base, int buf_size, int &p_file);

    // load data into input buffer
    void read_to_input(int file_base, int buf_size, int &p_file);

    // get the pivot
    void partition(int &left_pivot, int &right_pivot);

    // get the index queue
    void qsDeque(std::deque<int> &qs_deque, int data_size);

    // load data from middle memo to small memo
    void to_small(int file_base, int size, int &p_file);

    // load data from middle memo to large memo
    void to_large(int file_base, int size, int &p_file);

    // write into disk when small or large memo is full
    void write(T* buffer, int file_base, int size, int &p_file);

    // insert element into middle memo
    void replace();

    // sort recursively and store the elements into txt
    void sort();

    //TODO: 有时间就写
    // show the first elem_num elements of the result
    void show(int elem_num);
};

template<typename T>
void
ExternalQS<T>::read_to_mid(int file_base, int buf_size, int &p_file) {
    std::ifstream read_file{loc};
    int read_count = 0;
    assert(read_file.is_open());
    read_file.seekg(file_base, std::ios::beg); // read from 0 + file_base
    for (read_count; read_count < buf_size; ++read_count) {
        if (!read_file.eof()) {
            T temp;
            read_file >> temp;
            qs_queue->push(temp);
        }
    }
    p_file = read_file.tellg() / sizeof(T) - 1;// get the file pointer (the last index)
    read_file.close();
}

template<typename T>
void ExternalQS<T>::read_to_input(int file_base, int buf_size, int &p_file) {
    std::ifstream read_file{loc};
    int read_count = 0;
    assert(read_file.is_open());
    read_file.seekg(file_base, std::ios::beg); // read from 0 + file_base
    for (read_count; read_count < buf_size; ++read_count) {
        if (!read_file.eof()) {
            T temp;
            read_file >> temp;
            input_buf[read_count] = temp;
        }
    }
    p_file = read_file.tellg() / sizeof(T) - 1;// get the file pointer (the last index)
    read_file.close();
}

template<typename T>
void ExternalQS<T>::partition(int &left_pivot, int &right_pivot) {

}

template<typename T>
void ExternalQS<T>::qsDeque(std::deque<int> &qs_deque, int data_size) {
    qs_deque.push_front(0);
    qs_deque.push_back(data_size - 1);

    while (!qs_deque.empty()) {
        int left = qs_deque.front();
        int right = qs_deque.back();
        qs_deque.pop_front();
        qs_deque.pop_back();

        if (left < right) {
            int left_pivot;
            int right_pivot;
            partition(left_pivot, right_pivot);
            if (left < left_pivot) {
                qs_deque.push_front(left);
                qs_deque.push_back(left_pivot);
            }
            if (right > right_pivot) {
                qs_deque.push_front(right);
                qs_deque.push_back(right_pivot);
            }
        }
    }
}

template<typename T>
void ExternalQS<T>::to_small(int file_base, int size, int &p_file) {
    // the writing condition is confirmed by the function `replace`, which gives the file base and data size
}

template<typename T>
void ExternalQS<T>::to_large(int file_base, int size, int &p_file) {

}

template<typename T>
void ExternalQS<T>::write(T* buffer, int file_base, int size, int &p_file) {
    if (!buffer)
        return;
    std::ofstream write_file{loc};
    assert(write_file.is_open());
    write_file.seekp(file_base, std::ios::beg);
    for (int i = 0; i < size; ++i) {
        buffer[i] >> write_file;
    }
    p_file = write_file.tellp() / sizeof(T) - 1;
    write_file.close();
}

template<typename T>
void ExternalQS<T>::replace() {
     if (input_buf.empty()) {
         return;
     }
     T mid_max = qs_queue->getMax();
     T mid_min = qs_queue->getMin();
    while (input_buf.empty()) {
        T input_elem = input_buf.front();
        input_buf.pop();
        if (input_elem < mid_min) {
            if (small_buf.size() == SMALL_SIZE) {
                small_full = true;
            }
        }
    }
}

template<typename T>
void ExternalQS<T>::sort() {
    std::deque<int> indices;
    indices.push_front(0);  // storage left flag
    indices.push_back(qs_queue->Deque.size() - 1);  // storage right flag
    while (!indices.empty()) {
        int left = indices.front();
        int right = indices.back();
        int p_file = 0;
        indices.pop_front();
        indices.pop_back();

        read_to_mid(left, MIDDLE_SIZE, p_file);
        while ()
        read_to_input(p_file, INPUT_SIZE, p_file);
    }
}

template<typename T>
void ExternalQS<T>::show(int elem_num) {

}

template<typename T>
void
ExternalQS<T>::generate_rand(const std::string &type_name, int bottom, int top, int count) {
    std::ofstream write_file{loc};
    assert(write_file.is_open() && write_file.good());
    static std::default_random_engine generator(time(nullptr));
    if constexpr (std::is_integral<T>::value) { // confirm the type of data when compiling
        static std::uniform_int_distribution<T> rand_int(bottom, top);
        for (int i = 0; i < count; ++i) {
            T temp = rand_int(generator);
            write_file << temp << "\t";
            if (i % 100 == 0 && i != 0)
                write_file << "\n";
        }
    } else {
        static_assert(std::is_floating_point<T>::value, "Real Number required.");
        static std::uniform_real_distribution<T> rand_real(bottom, top);
        for (int i = 0; i < count; ++i) {
            T temp = rand_real(generator);
            write_file << temp << "\t";
            if (i % 100 == 0 && i != 0)
                write_file << "\n";
        }
    }
    write_file.close();
}