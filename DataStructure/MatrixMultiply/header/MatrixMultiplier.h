#pragma once

#include "Matrix.h"
#include "MatrixIO.h"

namespace Order {
    enum struct Sequence {
        I, J, K
    };

    struct Index {
        int *priority;
        int constrain;
        int value;

        Index &operator=(const Index &ind) {
            if (this != &ind) {
                this->constrain = ind.constrain;
                this->value = ind.value;
            }
            return *this;
        }
    };

    struct cmp {
        bool operator()(Index *lhs, Index *rhs) const {
            return lhs->priority < rhs->priority;
        }
    };

    int findPriority(std::vector<Sequence> seq, Sequence index) {
        for (auto it = seq.begin(); it != seq.end(); ++it) {
            if (*it == index) return (int) std::distance(seq.begin(), it);
        }
        return -1;
    }
}

template<typename T>
class MatrixMultiplier {
private:
    Matrix<T> matrix;
    MatrixIO<T> matrixIo;

public:
    MatrixMultiplier() = default;

    MatrixMultiplier(int row, int col, int cache_size, const std::string file_loc) {
        matrix.init(row, col);
        matrixIo.init(cache_size, file_loc);
    }

    MatrixMultiplier<T>
    multiply(MatrixMultiplier<T> &mm, std::vector<Order::Sequence> &mul_order, std::string store_file_loc);
};

template<typename T>
MatrixMultiplier<T> MatrixMultiplier<T>::multiply(MatrixMultiplier<T> &mm, std::vector<Order::Sequence> &mul_order,
                                                  std::string store_file_loc) {
//    this->matrix.matrixGenerator(this->matrixIo.getFileLoc());
//    mm.matrix.matrixGenerator(mm.matrixIo.getFileLoc());
    this->matrix.matrixTestGen();
    mm.matrix.matrixTestGen();
    int a = this->matrix.getCol();
    int b = mm.matrix.getRow();
    assert(a == b || !mul_order.empty());
    MatrixMultiplier<T> mat_c(this->matrix.getRow(), mm.matrix.getCol(),
                              this->matrixIo.getCacheSize(), store_file_loc);

    std::vector<std::pair<int, int> > indices(3, std::pair<int, int>(0, 0));

    // define constrain
    indices.at(0).second = this->matrix.getRow();
    indices.at(1).second = mm.matrix.getCol();
    indices.at(2).second = this->matrix.getCol();

    for (indices.at((int) mul_order.at(0)).first = 0;
         indices.at((int) mul_order.at(0)).first < indices.at((int) mul_order.at(0)).second;
         ++indices.at((int) mul_order.at(0)).first) {
        for (indices.at((int) mul_order.at(1)).first = 0;
             indices.at((int) mul_order.at(1)).first < indices.at((int) mul_order.at(1)).second;
             ++indices.at((int) mul_order.at(1)).first) {
            for (indices.at((int) mul_order.at(2)).first = 0;
                 indices.at((int) mul_order.at(2)).first < indices.at((int) mul_order.at(2)).second;
                 ++indices.at((int) mul_order.at(2)).first) {
                mat_c.matrix[indices[0].first][indices[1].first] +=
                        this->matrix[indices[0].first][indices[2].first] *
                        mm.matrix[indices[2].first][indices[1].first];
            }
        }
    }
    this->matrix.printMatrix();
    std::cout << std::endl;
    mm.matrix.printMatrix();
    std::cout << std::endl;
    mat_c.matrix.printMatrix();
    return mat_c;
}
