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
    T empty_flag_;
    int invalid_num_;
    std::vector<T> buf_;

public:
    Buffer() {
        invalid_num_ = 0;
        empty_flag_ = std::numeric_limits<T>::max();
    }

    explicit Buffer(int size) {
        invalid_num_ = 0;
        buf_.assign(size, 0);
        empty_flag_ = std::numeric_limits<T>::max();
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

    int getInvalidNum() {
        return invalid_num_;
    }

    void read_n(std::string &in_file, long long &start_p, long long &read_bytes);

    void write_n(std::string &out_file, long long &start_p, long long &write_bytes);

    static bool greaterThan(T a, T b) { return a < b; }

    // use back, the back is the first value
    void ascendSort();

    void tagEmpty() {
        buf_.pop_back();
        buf_.push_back(empty_flag_);
    }

};

template<typename T>
void Buffer<T>::read_n(std::string &in_file, long long &start_p, long long &read_bytes) {
    std::fstream read_file{in_file, std::ios::in | std::ios::out | std::ios::binary};
    assert(read_file.is_open() && read_file.good());
    read_file.seekg(start_p, std::fstream::beg);
    read_file.read(reinterpret_cast<char *>(buf_.data()), read_bytes);
    start_p = read_file.tellg();
    read_file.close();
}

template<typename T>
void Buffer<T>::write_n(std::string &out_file, long long &start_p, long long &write_bytes) {
    std::fstream write_file{out_file, std::ios::out | std::ios::in | std::ios::binary};
    assert(write_file.is_open() && write_file.good());
    write_file.seekp(start_p, std::fstream::beg);
    write_file.write(reinterpret_cast<const char *>(buf_.data()), write_bytes);
    buf_.assign(size(), empty_flag_);
    start_p = write_file.tellp();
    write_file.close();
}

template<typename T>
void Buffer<T>::ascendSort() {
    std::sort(buf_.begin(), buf_.end() - invalid_num_, greaterThan);
}


