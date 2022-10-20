#pragma once

/*
 * Buffer class
 * add IO operation based on STL vector
 * includes Input buffer, Loser Tree buffer, Output buffer
 */

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>

template<typename T>
class Buffer {
private:
    std::vector<T> buf_;

public:
    Buffer() = default;

    explicit Buffer(int size) {
        buf_.assign(size, 0);
    }

    ~Buffer() = default;

    T &operator[](int ind) {
        return buf_[ind];
    }

    const T &operator[](int ind) const {
        return buf_[ind];
    }

    int size() {
        return buf_.size();
    }

    bool empty() {
        return buf_.empty();
    }

    void push_back(T value) {
        buf_.push_back(value);
    }

    void readFill(std::fstream read_file, long long &read_p);

    void writeAll(std::fstream write_file, long long &write_p);

    void ascendSort();

};

template<typename T>
void Buffer<T>::readFill(std::fstream read_file, long long &read_p) {
    assert(read_file.is_open() && read_file.good());
    read_file.read(reinterpret_cast<char *>(buf_), sizeof(buf_));
    read_p = read_file.tellg();
}

template<typename T>
void Buffer<T>::writeAll(std::fstream write_file, long long &write_p) {
    assert(write_file.is_open() &&write_file.good());
    write_file.seekp(write_p, std::ios::beg);
    write_file.write(reinterpret_cast<const char *>(buf_), sizeof(buf_));
    write_p = write_file.tellp();
}

template<typename T>
void Buffer<T>::ascendSort() {
    std::sort(buf_.begin(), buf_.end());
}
