#pragma once

/*
 * Buffer class
 * add IO operation based on STL vector
 * includes Input buffer, Loser Tree buffer, Output buffer
 */
#include "LoserTree.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <limits>
#include <iostream>

template<typename T>
class Buffer {
private:
    T empty_flag_;
    std::vector<T> buf_;

public:
    Buffer() {
        empty_flag_ = std::numeric_limits<T>::max();
    }

    explicit Buffer(int size) {
        empty_flag_ = std::numeric_limits<T>::max();
        buf_.assign(size, empty_flag_);
    }

    ~Buffer() = default;

    void init(int size) {
        buf_.assign(size, empty_flag_);
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

    T back() const {
        return buf_.back();
    }

    void read_n(std::string &in_file, long long &start_p, long long &read_bytes) {
        std::fstream read_file{in_file, std::ios::in | std::ios::out | std::ios::binary};
        assert(read_file.is_open() && read_file.good());
        read_file.seekg(start_p, std::fstream::beg);
        read_file.read(reinterpret_cast<char *>(buf_.data()), read_bytes);
        start_p = read_file.tellg();
        read_file.close();
    }

    void write_n(std::string &out_file, long long &start_p, long long &write_bytes) {
        std::fstream write_file{out_file, std::ios::out | std::ios::in | std::ios::binary};
        assert(write_file.is_open() && write_file.good());
        write_file.seekp(start_p, std::fstream::beg);
        write_file.write(reinterpret_cast<const char *>(buf_.data()), write_bytes);
        buf_.assign(size(), empty_flag_);
        start_p = write_file.tellp();
        write_file.close();
    }

    static bool greaterThan(T a, T b) { return a < b; }

    // use back, the back is the first value
    void ascendSort() {
        std::sort(buf_.begin(), buf_.end(), greaterThan);
    }

    // tag from loc to tail is illegal area
    void tagEmpty(int loc) {
        for (int i = loc; i < buf_.size(); ++i) {
            buf_[i] = empty_flag_;
        }
    }

    void makeEmpty() {
        buf_.assign(buf_.size(), empty_flag_);
    }

    bool isFree() {
        for (T &value: buf_) {
            if (value != empty_flag_) {
                return false;
            }
        }
        return true;
    }

    bool isFull() {
        for (T &value: buf_) {
            if (value == empty_flag_) {
                return false;
            }
        }
        return true;
    }


    friend
    std::ostream &operator<<(std::ostream &stream, const Buffer<T> &buf) {
        for (int i = 0; i < buf.buf_.size(); ++i) {
            stream << buf.buf_[i] << " ";
        }
        stream << std::endl;
        return stream;
    }
};

