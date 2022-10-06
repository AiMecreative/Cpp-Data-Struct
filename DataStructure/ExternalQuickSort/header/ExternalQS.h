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
    std::deque<T> input_buf;
    std::deque<T> small_buf;
    std::deque<T> large_buf;
public:
    PriorityDeque<T> *qs_queue; // middle

    const std::string loc;
    const std::string temp_loc;
    const std::string result_loc;

    ExternalQS() {
        qs_queue = new PriorityDeque<T>;
    }

    ~ExternalQS() = default;

    // generate rand number and write to txt
    void generate_rand(const std::string &type_name,
                       int bottom, int top, int count);

    // read in data from txt, `p_file` is the tail of file
    void read_to_mid(int file_base, int buf_size, int &p_file);

    // load data into input buffer
    void read_to_input(int file_base, int buf_size, int &p_file);

    // write into disk when small or large memo is full
    void write(const std::string &loc, std::deque<T> buffer, int file_base, int size, int &p_file);

    // write temp_file to result_file
    void write_t2r(const std::string &loc_result, const std::string &loc_temp, int file_base, int &p_file);

    // insert element into middle memo
    void replace(const std::string &loc_result, const std::string &loc_temp, int &p_small, int &p_large);

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
    for (read_count; read_count < buf_size || !read_file.eof(); ++read_count) {
        T temp;
        read_file >> temp;
        qs_queue->push(temp);
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
    for (read_count; read_count < buf_size && !read_file.eof(); ++read_count) {
        T temp;
        read_file >> temp;
        input_buf.push_front(temp);
    }
    p_file = read_file.tellg() / sizeof(T) - 1; // get the file pointer (the last index)
    read_file.close();
}

template<typename T>
void ExternalQS<T>::write(const std::string &_loc, std::deque<T> buffer, int file_base, int size, int &p_file) {
    if (buffer.empty())
        return;
    std::ofstream write_file{_loc};
    assert(write_file.is_open());
    write_file.seekp(file_base, std::ios::beg);
    for (auto it = buffer.begin(); it != buffer.end(); ++it) {
        write_file << *it;
    }
    p_file = write_file.tellp() / sizeof(T) - 1;
    write_file.close();
}

template<typename T>
void ExternalQS<T>::write_t2r(const std::string &loc_result, const std::string &loc_temp, int file_base, int &p_file) {
    std::ofstream result_file{loc_result};
    std::ifstream temp_file{loc_temp};
    assert(result_file.is_open() || temp_file.is_open());
    result_file.seekp(file_base, std::ios::beg);
    while (!temp_file.eof()) {
        T elem;
        temp_file >> elem;
        result_file << elem;
    }
    p_file = result_file.tellp() / sizeof(T) - 1;
    result_file.close();
    temp_file.close();
}

template<typename T>
void ExternalQS<T>::replace(const std::string &loc_result, const std::string &loc_temp, int &p_small, int &p_large) {
    // the input two pointers means where to start
    // the return of the two pointers means the size
    if (input_buf.empty()) {
        return;
    }
    std::ofstream result_file{loc_result};
    std::ifstream temp_file{loc_temp};
    bool flag = false; // used for change min or max in middle_buf, false--min, true--max
    T mid_max = qs_queue->getMax();
    T mid_min = qs_queue->getMin();
    while (!input_buf.empty()) {
        T input_elem = input_buf.front();
        input_buf.pop_front();
        if (input_elem < mid_min) {
            small_buf.push_front(input_elem);
            // write back the small part when small_buf is full
            if (small_buf.size() == SMALL_SIZE) {
                // small_buf write to result file
                write(result_loc, small_buf, p_small, small_buf.size(), p_small);
            }
        }
        else if (input_elem > mid_max) {
            large_buf.push_front(input_elem);
            // write back the large part when large_buf is full or all data has been detected
            if (large_buf.size() == LARGE_SIZE) {
                // large_buf write to temp file, write to result file after middle data is written
                write(temp_loc, large_buf, p_large, large_buf.size(), p_large);
            }
        }
        else {
            //TODO: mid_min < input_elem < mid_max
            if (qs_queue->Deque.size() == MIDDLE_SIZE) {
                if (flag) {
                    small_buf.push_front(qs_queue->getMin());
                    qs_queue->popMin();
                    flag = false;
                } else {
                    large_buf.push_front(qs_queue->getMax());
                    qs_queue->popMax();
                    flag = true;
                }
                qs_queue->push(input_elem);
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
        indices.pop_front();
        indices.pop_back();

        int p_file = 0;  // used for original file
        int p_small = 0; // used for write small to file
        int p_large = 0; // used for write large to file
        read_to_mid(left, MIDDLE_SIZE, p_file);

        while (p_file < right) {
            read_to_input(p_file, INPUT_SIZE, p_file);
            replace(result_loc, temp_loc, p_small, p_large);

            // if all data have been detected, must be written
            // file is over but there are still elements in small_buf
            if (p_file == right && p_small != SMALL_SIZE) {
                write(result_loc, small_buf, p_small, small_buf.size(), p_small);
            }

            // file is over but there are still elements in large_buf
            if (p_file == right && p_large != LARGE_SIZE) {
                write(temp_loc, large_buf, p_large, large_buf.size(), p_large);
            }
        }

        p_file = p_small;
        qs_queue->write(result_loc, p_file, qs_queue->Deque.size(), p_file);

        p_large = p_file;
        write_t2r(result_loc, temp_loc, p_file, p_file);

        // after all data has been detected, partition the small and large part again
        indices.push_front(left);
        indices.push_back(p_small);
        indices.push_front(p_large);
        indices.push_back(right);
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