#include <iostream>
#include "MatrixMultiplier.h"

int main() {
    const std::string file_A = "./data/matrix_A.txt";
    const std::string file_B = "./data/matrix_B.txt";
    const std::string file_C = "./data/matrix_C.txt";

    MatrixMultiplier<int> matrix_A(2, 2, 100, file_A);
    MatrixMultiplier<int> matrix_B(2, 2, 100, file_B);
    MatrixMultiplier<int> matrix_C;

    std::vector<Order::Sequence> mul_order = {Order::Sequence::I, Order::Sequence::J, Order::Sequence::K};

    matrix_C = matrix_A.multiply(matrix_B, mul_order, file_C);
}
