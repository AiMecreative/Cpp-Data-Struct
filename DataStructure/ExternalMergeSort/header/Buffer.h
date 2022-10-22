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
    int invalid_num_;
    std::vector<T> buf_;

public:
    Buffer() {
        invalid_num_ = 0;
    }

    explicit Buffer(int size) {
        invalid_num_ = 0;
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

    void readFill(const std::string &in_file, long long &read_p, long long &file_size);

    void writeAll(const std::string &out_file, long long &write_p, long long &file_size);

    static bool greaterThan(T a, T b) { return a < b; }

    // use back, the back is the first value
    void ascendSort();

};

template<typename T>
void Buffer<T>::readFill(const std::string &in_file, long long &read_p, long long &file_size) {
    std::fstream read_file{in_file, std::ios::in | std::ios::out | std::ios::binary};
    assert(read_file.is_open() && read_file.good());

    int read_bytes = std::min((long long)(size() * sizeof(T)), file_size - read_p);

    invalid_num_ = size() - read_bytes / sizeof(T);

    read_file.seekg(read_p, std::fstream::beg);
    read_file.read(reinterpret_cast<char *>(buf_.data()), read_bytes);
    read_p = read_file.tellg();
    read_file.close();
}

template<typename T>
void Buffer<T>::writeAll(const std::string &out_file, long long &write_p, long long &file_size) {
    std::fstream write_file{out_file, std::ios::out | std::ios::in | std::ios::binary};
    assert(write_file.is_open() && write_file.good());
    write_file.seekp(write_p, std::fstream::beg);

    int write_bytes = std::min((long long)(size() * sizeof(T)), file_size - write_p);

    write_file.write(reinterpret_cast<const char *>(buf_.data()), write_bytes);
    buf_.assign(size(), 0);
    write_p = write_file.tellp();
    write_file.close();
}

template<typename T>
void Buffer<T>::ascendSort() {
    std::sort(buf_.begin(), buf_.end() - invalid_num_, greaterThan);
}

