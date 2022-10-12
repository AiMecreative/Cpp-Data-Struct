#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <random>

enum class order {
    I, J ,K
};

template<typename T>
struct is_int {
    constexpr explicit operator bool() {
        return false;
    }
};

template<>
struct is_int<int> {
    constexpr explicit operator bool() {
        return true;
    }
};

template<typename T>
class Matrix {
private:
    int row;
    int col;
    std::vector< std::vector<T> > matrix;

public:
    Matrix() {
        row = 0;
        col = 0;
    };
    Matrix(int row, int col);
    Matrix(Matrix<T> &mat);
    ~Matrix() = default;

    int getRow();
    int getCol();
    void matrixGenerator(std::string & file_loc, int row, int col, int top=0, int bottom=10);
    Matrix<T> multiply(Matrix<T>& mat, std::vector<int>& mul_order);
};

template<typename T>
Matrix<T>::Matrix(int row, int col) {
    this->row = row;
    this->col = col;
    for (int i = 0; i < this->row; ++i) {
        matrix[i].assign(this->col, 0);
    }
}

template<typename T>
Matrix<T>::Matrix(Matrix<T> &mat) {
    this->row = mat.row;
    this->col = mat.col;
    this->matrix = mat.matrix;
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
void Matrix<T>::matrixGenerator(std::string &file_loc, int _row, int _col, int top, int bottom) {
    std::fstream write_file{file_loc, std::ios::out | std::ios::in};
    assert(write_file.is_open() && write_file.good());
    std::default_random_engine generator(T);
    if (static_cast<bool>(is_int<T>())) {
        std::uniform_int_distribution<T> uniform(top, bottom);
        for (int i = 0; i < _row; ++i) {
            for (int j = 0; j < _col; ++j) {
                write_file << uniform(generator) << "\t";
            }
            write_file << "\n";
        }
    } else {
        std::uniform_real_distribution<T> uniform(top, bottom);
        for (int i = 0; i < _row; ++i) {
            for (int j = 0; j < _col; ++j) {
                write_file << uniform(generator) << "\t";
            }
            write_file << "\n";
        }
    }
}

template<typename T>
Matrix<T> Matrix<T>::multiply(Matrix<T> &mat, std::vector<int>& mul_order) {
    assert(this->col == mat.row || mul_order.empty());
    int outer = mul_order.at(0);
    int mid = mul_order.at(1);
    int inner = mul_order.at(2);
    //TODO: 如何确定for循环的顺序
}

