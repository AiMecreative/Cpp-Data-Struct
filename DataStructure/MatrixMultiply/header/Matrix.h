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
    std::vector<std::vector<T> > matrix;

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

    void matrixTestGen(int bottom = 0, int top = 10);

    void printMatrix();

    std::vector<T> operator[](int i) const { return matrix[i]; }

    std::vector<T> &operator[](int i) { return matrix[i]; }
};

template<typename T>
void Matrix<T>::init(int _row, int _col) {
    int _init = 0;
    this->row = _row;
    this->col = _col;
    this->matrix.assign(row, std::vector<T>(col, *(T *) &_init));
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
    std::fstream write_file{file_loc, std::ios::in | std::ios::out};
    assert(write_file.good());
    write_file.seekp(0, std::ios::beg);
    std::default_random_engine generator((unsigned int) time(nullptr));
    if constexpr (static_cast<bool>(is_int<T>())) {
        std::uniform_int_distribution<T> uniform(top, bottom);
        for (int i = 0; i < this->row; ++i) {
            for (int j = 0; j < this->col; ++j) {
                write_file << uniform(generator) << "\t";
            }
            write_file << "\n";
        }
    } else {
        std::uniform_real_distribution<T> uniform(top, bottom);
        for (int i = 0; i < this->row; ++i) {
            for (int j = 0; j < this->col; ++j) {
                write_file << uniform(generator) << "\t";
            }
            write_file << "\n";
        }
    }
    write_file.close();
}

template<typename T>
void Matrix<T>::printMatrix() {
    assert(!matrix.empty());
    for (auto it0 = matrix.begin(); it0 != matrix.end(); ++it0) {
        for (auto it1 = it0->begin(); it1 != it0->end(); ++it1) {
            std::cout << *it1 << "\t";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void Matrix<T>::matrixTestGen(int bottom, int top) {
    static std::default_random_engine gen((unsigned int) time(nullptr));
    if constexpr (static_cast<bool>(is_int<T>())) {
        static std::uniform_int_distribution<T> uniform(bottom, top);
        for (auto &it0: matrix) {
            for (auto &it1: it0) {
                it1 = uniform(gen);
            }
        }
    } else {
        static std::uniform_real_distribution<T> uniform(bottom, top);
        for (auto &it0: matrix) {
            for (auto &it1: it0) {
                it1 = uniform(gen);
            }
        }
    }
}


