#pragma once

#include "Matrix.h"
#include <vector>
#include <fstream>
#include <cassert>

template<typename T>
class MatrixIO : public Matrix<T> {
private:
    int cacheSize{};
    std::vector<T> cacheLine;

    std::vector<long long> read_p;
    std::vector<long long> write_p;

    int readBlockNum;  // the record of the number of blocks have been read this turn
    int writeBlockNum; //

    int readMissTimes;
    int writeMissTimes;

public:
    using Matrix<T>::Matrix;

    MatrixIO() {
        int _init = 0;
        this->cacheSize = 0;
        this->cacheLine.assign(this->cacheSize, *(T *) &_init);
        readBlockNum = 0;
        writeBlockNum = 0;
        read_p.push_back(std::ifstream::beg);
        write_p.push_back(std::ifstream::beg);
        readMissTimes = 0;
        writeMissTimes = 0;
    }

    ~MatrixIO() = default;

    void init(int row, int col, int cache_size);

    void setCacheSize(int cache_size);

    // file to cache
    void cacheRead(const std::string &file_loc);

    // cache to file
    void cacheWrite(const std::string &file_loc);

    // cache to matrix single value
    T singleReadCache(const std::string &file_loc, int row_i, int col_j);

    // matrix to cache single value
    void singleWriteCache(const std::string &file_loc, T value, int row_i, int col_j);

    std::pair<int, int> getMissTimes();

    int getCacheSize();

    bool emptyCache();
};

template<typename T>
void MatrixIO<T>::init(int row, int col, int cache_size) {
    Matrix<T>::init(row, col);
    int init = 0;
    this->cacheSize = cache_size;
    this->cacheLine.assign(this->cacheSize, *(T *) &init);
    readBlockNum = 0;
    writeBlockNum = 0;
    read_p.push_back(std::ifstream::beg);
    write_p.push_back(std::ifstream::beg);
}

template<typename T>
void MatrixIO<T>::setCacheSize(int cache_size) {
    int init = 0;
    this->cacheSize = cache_size;
    this->cacheLine.clear();
    this->acheLine.assign(this->cacheSize, *(T *) &init);
    readBlockNum = 0;
    writeBlockNum = 0;
    read_p.push_back(std::ifstream::beg);
    write_p.push_back(std::ifstream::beg);
}

template<typename T>
void MatrixIO<T>::cacheRead(const std::string &file_loc) {
    std::ifstream read_file{file_loc, std::ios::in};
    assert(read_file.is_open());
    read_file.seekg(read_p.back());
    for (int i = 0; i < this->cacheSize; ++i) {
        read_file >> this->cacheLine[i];
        if (read_file.peek() == EOF) {
            read_file.seekg(read_p.front());
        }
    }
    read_p.push_back(read_file.tellg());
    read_file.close();
}

template<typename T>
void MatrixIO<T>::cacheWrite(const std::string &file_loc) {
    std::fstream write_file{file_loc, std::ios::in | std::ios::out};
    assert(write_file.is_open());
    write_file.seekp(write_p.back());
    for (auto it = cacheLine.begin(); it != cacheLine.end(); ++it) {
        write_file << *it << "\t";
        *it = 0;
    }
    write_p.push_back(write_file.tellg());
    write_file.close();
}

template<typename T>
T MatrixIO<T>::singleReadCache(const std::string &file_loc, int row_i, int col_j) {
    assert(!this->cacheLine.empty());
    int row = this->getRow();
    int col = this->getCol();
    assert(row > 0 && col > 0);
    int cache_ind = row_i * col + col_j - this->readBlockNum * this->cacheSize;
    if (0 <= cache_ind && cache_ind < this->cacheSize) {
        return this->cacheLine[cache_ind];
    } else if (cache_ind >= this->cacheSize) {
        // update the cacheLine value when cache_ind is greater than current cacheSize
        // until cache_ind < cache_ind
        while (cache_ind >= this->cacheSize) {
            this->readBlockNum += 1;
            cache_ind -= this->cacheSize;
            cacheRead(file_loc); // update cache value, file->cache
        }
        return this->cacheLine[cache_ind];
    } else {
        //cache_ind < 0
        // roll back to former file_p one by one, until cache_ind >= 0
        readMissTimes += readBlockNum;
        while (cache_ind < 0) {
            read_p.pop_back();
            read_p.pop_back();
            readBlockNum -= 1;
            writeMissTimes += 1;
            cache_ind += cacheSize;
            cacheRead(file_loc);
        }
        return this->cacheLine[cache_ind];
    }
}

template<typename T>
void MatrixIO<T>::singleWriteCache(const std::string &file_loc, T value, int row_i, int col_j) {
    assert(!this->cacheLine.empty());
    int row = this->getRow();
    int col = this->getCol();
    assert(row > 0 && col > 0);
    int cache_ind = row_i * col + col_j - this->writeBlockNum * this->cacheSize;
    if (0 <= cache_ind && cache_ind < this->cacheSize) {
        this->cacheLine[cache_ind] += value;
    } else if (cache_ind >= this->cacheSize) {
        // update the cacheLine value when cache_ind >= cacheSize
        // write the cache before it is read, as the value in cacheLine is not changed
        while (cache_ind >= this->cacheSize) {
            this->writeBlockNum += 1;
            cache_ind -= this->cacheSize;
            cacheWrite(file_loc);
            // the write_p is changed, but we must keep the read_p the same
            // so use a new pointer
            long long temp_p = write_p.back();
            std::ifstream read_file{file_loc};
            read_file.seekg(temp_p, std::ios::beg);
            for (int i = 0; i < this->cacheSize; ++i) {
                read_file >> this->cacheLine[i];
                if (read_file.peek() == EOF) {
                    read_file.seekg(std::ifstream::beg);
                }
            }
        }
        this->cacheLine[cache_ind] += value;
    } else {
        // cache_ind < 0
        while (cache_ind < 0) {
            writeBlockNum -= 1;
            writeMissTimes += 1;

            cacheWrite(file_loc);
            write_p.pop_back();
            write_p.pop_back();

            long long temp_p = write_p.back();
            std::ifstream read_file{file_loc};
            read_file.seekg(temp_p, std::ios::beg);
            for (int i = 0; i < cacheSize; ++i) {
                read_file >> cacheLine[i];
            }
            cache_ind += cacheSize;
        }
        this->cacheLine[cache_ind] += value;
    }
}

template<typename T>
std::pair<int, int> MatrixIO<T>::getMissTimes() {
    this->readMissTimes += this->readBlockNum;
    this->writeMissTimes += this->writeBlockNum;
    return {this->readMissTimes, this->writeMissTimes};
}

template<typename T>
int MatrixIO<T>::getCacheSize() {
    return cacheSize;
}

template<typename T>
bool MatrixIO<T>::emptyCache() {
    return cacheLine.empty();
}

