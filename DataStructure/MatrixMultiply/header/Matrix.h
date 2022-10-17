#pragma once

#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <cassert>
#include <random>
#include <iostream>

template<typename T>
constexpr bool is_int() {
    return false;
}

template<>
constexpr bool is_int<int>() {
    return true;
}

template<typename T>
class Matrix {
private:
    int row;
    int col;

public:
    Matrix() {
        row = 0;
        col = 0;
    };

    ~Matrix() = default;

    int getRow();

    int getCol();

    void init(int row, int col);

    void matrixGenerator(const std::string &file_loc, int bottom = 0, int top = 10);

};

template<typename T>
void Matrix<T>::init(int _row, int _col) {
    this->row = _row;
    this->col = _col;
}

template<typename T>
int Matrix<T>::getRow() {
    return this->row;
}

template<typename T>
int Matrix<T>::getCol() {
    return this->col;
}

template<typename T>
void Matrix<T>::matrixGenerator(const std::string &file_loc, int bottom, int top) {
    std::ofstream write_file{file_loc};
    assert(write_file.is_open());
    static std::default_random_engine generator((unsigned int) time(nullptr));
    if constexpr (static_cast<bool>(is_int<T>())) {
        static std::uniform_int_distribution<T> uniform(bottom, top);
        for (int i = 0; i < this->row; ++i) {
            for (int j = 0; j < this->col; ++j) {
                T elem = uniform(generator);
                write_file << elem << "\t";
            }
        }
    } else {
        static std::uniform_real_distribution<T> uniform(bottom, top);
        for (int i = 0; i < this->row; ++i) {
            for (int j = 0; j < this->col; ++j) {
                T elem = uniform(generator);
                write_file << elem << "\t";
            }
        }
    }
    write_file.close();
}
