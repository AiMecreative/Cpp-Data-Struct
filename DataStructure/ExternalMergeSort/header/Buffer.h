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
#include <limits>

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

    void init(int size) {
        buf_.assign(size, 0);
    }

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

    // use back
    T back() const {
        return buf_.back();
    }

    void pop_back() {
        buf_.pop_back();
    }

    void readFill(const std::string &in_file, long long &read_p);

    void writeAll(const std::string &out_file, long long &write_p);

    static bool greaterThan(T a, T b) { return a > b; }

    // use back, the back is the first value
    void ascendSort();

};

template<typename T>
void Buffer<T>::readFill(const std::string &in_file, long long &read_p) {
    std::fstream read_file{in_file, std::ios::in | std::ios::binary};
    assert(read_file.is_open() && read_file.good());
    read_file.read(reinterpret_cast<char *>(&buf_[0]), sizeof(buf_));
    read_p = read_file.tellg();
}

template<typename T>
void Buffer<T>::writeAll(const std::string &out_file, long long &write_p) {
    std::fstream write_file{out_file, std::ios::out | std::ios::binary};
    assert(write_file.is_open() && write_file.good());
    write_file.seekp(write_p, std::ios::beg);
    write_file.write(reinterpret_cast<const char *>(&buf_[0]), sizeof(buf_));
    write_p = write_file.tellp();
}

template<typename T>
void Buffer<T>::ascendSort() {
    // return 3 > 2 > 1
    std::sort(buf_.begin(), buf_.end(), greaterThan);
}

