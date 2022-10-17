#pragma once

#include "Matrix.h"
#include "MatrixIO.h"

namespace Order {
    enum struct Sequence {
        I, J, K
    };
}

template<typename T>
class MatrixMultiplier : public MatrixIO<T> {
public:
    using MatrixIO<T>::MatrixIO;

    MatrixMultiplier() = default;

    MatrixMultiplier(int row, int col, int cache_size) : MatrixIO<T>() {
        MatrixIO<T>::init(row, col, cache_size);
    }

    void
    multiply(MatrixMultiplier<T> &mat_b,
             MatrixMultiplier<T> &mat_c,
             std::vector<Order::Sequence> &mul_order,
             const std::string &file_A,
             const std::string &file_B,
             const std::string &file_C);
};

template<typename T>
void
MatrixMultiplier<T>::multiply(MatrixMultiplier<T> &mat_b,
                              MatrixMultiplier<T> &mat_c,
                              std::vector<Order::Sequence> &mul_order,
                              const std::string &file_A,
                              const std::string &file_B,
                              const std::string &file_C) {

    this->matrixGenerator(file_A);
    mat_b.matrixGenerator(file_B);

    this->cacheRead(file_A);
    mat_b.cacheRead(file_B);

    int a = this->getCol();
    int b = mat_b.getRow();

    if ((a != b) || mul_order.empty()) {
        throw std::runtime_error(
                "can't multiply two matrix with different row and col! or you must pass a non-empty mul-order");
    }

    std::vector<std::pair<int, int> > indices(3, std::pair<int, int>(0, 0));

    // define constrain
    indices.at(0).second = this->getRow();
    indices.at(1).second = mat_b.getCol();
    indices.at(2).second = this->getCol();

    for (indices.at((int) mul_order.at(0)).first = 0;
         indices.at((int) mul_order.at(0)).first < indices.at((int) mul_order.at(0)).second;
         ++indices.at((int) mul_order.at(0)).first) {

        for (indices.at((int) mul_order.at(1)).first = 0;
             indices.at((int) mul_order.at(1)).first < indices.at((int) mul_order.at(1)).second;
             ++indices.at((int) mul_order.at(1)).first) {

            for (indices.at((int) mul_order.at(2)).first = 0;
                 indices.at((int) mul_order.at(2)).first < indices.at((int) mul_order.at(2)).second;
                 ++indices.at((int) mul_order.at(2)).first) {

//                output of indices
//                std::cout << indices[0].first << indices[2].first << std::endl;
//                std::cout << indices[2].first << indices[1].first << std::endl;
//                std::cout << indices[0].first << indices[1].first << std::endl;
//                std::cout << std::endl;

                T a_value = this->singleReadCache(file_A, indices[0].first, indices[2].first);
                T b_value = mat_b.singleReadCache(file_B, indices[2].first, indices[1].first);

                T c_value = a_value * b_value;

                mat_c.singleWriteCache(file_C, c_value, indices[0].first, indices[1].first);
            }
        }
    }

    // check if there is any value in cache
    if (!mat_c.emptyCache()) {
        mat_c.cacheWrite(file_C);
    }
}
