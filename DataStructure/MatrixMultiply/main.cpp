#include <iostream>
#include "MatrixMultiplier.h"

int main() {
    const std::string file_A = R"(D:\xrCLang\Self-Study-For-Cpp\DataStructure\MatrixMultiply\data\matrix_A.txt)";
    const std::string file_B = R"(D:\xrCLang\Self-Study-For-Cpp\DataStructure\MatrixMultiply\data\matrix_B.txt)";
    const std::string file_C = R"(D:\xrCLang\Self-Study-For-Cpp\DataStructure\MatrixMultiply\data\matrix_C.txt)";

    // file examination
    std::vector<std::string> file_vec = {file_A, file_B, file_C};
    for (auto &it: file_vec) {
        std::ofstream exam{it};
        assert(exam.is_open());
        exam.close();
    }
//    const std::string file_A = "matrix_A.txt";
//    const std::string file_B = "matrix_B.txt";
//    const std::string file_C = "matrix_C.txt";

    MatrixMultiplier<int> matrix_A(1, 1, 2);
    MatrixMultiplier<int> matrix_B(1, 1, 2);
    MatrixMultiplier<int> matrix_C(1, 1, 2);

    std::vector<Order::Sequence> mul_order = {Order::Sequence::I, Order::Sequence::J, Order::Sequence::K};

    matrix_A.multiply(matrix_B, matrix_C, mul_order, file_A, file_B, file_C);
}
