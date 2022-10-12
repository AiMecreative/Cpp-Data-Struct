#pragma once

#include "Matrix.h"
#include <vector>
#include <fstream>
#include <cassert>

template<typename T>
class MatrixIO {
private:
    int cacheSize;
    std::vector<T> cacheLine;

    long long read_p;
    long long write_p;

    int missTimes;

    std::string file_loc;

public:
    MatrixIO() = default;

    MatrixIO(int cache_size, std::string file_loc);

    ~MatrixIO() = default;

    void setCacheSize(int cache_size);

    void cacheRead();

    void cacheWrite(int row);

    void singleReadCache(Matrix<T> &mat, int row_i, int col_j);

    void singleWriteCache(Matrix<T> &mat, int row_i, int col_j);

    int getMissTimes() const;
};

template<typename T>
MatrixIO<T>::MatrixIO(int cache_size, std::string file_loc) {
    int init = 0;
    this->cacheSize = cache_size;
    this->cacheLine.assign(this->cacheSize, *(T *) &init);
    read_p = 0;
    write_p = 0;
    missTimes = 0;
    this->file_loc = file_loc;
}

template<typename T>
void MatrixIO<T>::setCacheSize(int cache_size) {
    int init = 0;
    this->cacheSize = cache_size;
    this->cacheLine.clear();
    this->acheLine.assign(this->cacheSize, *(T *) &init);
    read_p = 0;
    write_p = 0;
    missTimes = 0;
}

template<typename T>
void MatrixIO<T>::cacheRead() {
    std::fstream read_file{file_loc, std::ios::in | std::ios::out};
    assert(read_file.is_open() && read_file.good());
    read_file.seekg(this->read_p);
    for (auto it = cacheLine.begin(); it != cacheLine.end(); ++it) {
        read_file >> *it;
    }
    this->read_p = read_file.tellg();
    read_file.close();
}

template<typename T>
void MatrixIO<T>::cacheWrite(int row) {
    std::fstream write_file{file_loc, std::ios::in | std::ios::out};
    assert(write_file.is_open() && write_file.good());
    write_file.seekp(this->write_p);
    auto it0 = cacheLine.begin();
    for (auto it = it0; it != cacheLine.end(); ++it) {
        write_file << *it << "\t";
        if ((it - it0) % row == 0) {
            write_file << "\n";
        }
    }
    this->write_p = write_file.tellp();
    write_file.close();
}

template<typename T>
void MatrixIO<T>::singleReadCache(Matrix<T> &mat, int row_i, int col_j) {
    assert(this->cacheLine.empty());
    int row = mat.getRow();
    int col = mat.getCol();
    assert(row <= 0 || col <= 0);
    int cache_ind = row_i * row + col_j - this->missTimes * cacheSize;
    if (cache_ind < this->cacheSize) {
        mat[row_i][col_j] = this->cacheLine[cache_ind];
    } else {
        // row_i * row + col_j >= cacheSize
        cacheRead();
        this->missTimes += 1;
        cache_ind  = row_i * row + col_j - this->missTimes * cacheSize;
        mat[row_i][col_j] = this->cacheLine[cache_ind];
    }
}

template<typename T>
void MatrixIO<T>::singleWriteCache(Matrix<T> &mat, int row_i, int col_j) {
    assert(this->cacheLine.empty());
    int row = mat.getRow();
    int col = mat.getCol();
    assert(row <= 0 && col <= 0);
    //TODO: 写的时候如何定位, 满足可以在任意地方写入
}

