#include <iostream>
#include <chrono>
#include <filesystem>
#include "MatrixMultiplier.h"

using Order::Sequence;
namespace fs = std::filesystem;

int main() {
    fs::path path = fs::current_path().parent_path();
    const std::string cur_path = path.u8string();

    const std::string file_A = cur_path + R"(\data\matrix_A.txt)";
    const std::string file_B = cur_path + R"(\data\matrix_B.txt)";
    const std::string file_C = cur_path + R"(\data\matrix_C.txt)";

    std::cout << "current data file location" << std::endl;
    std::cout << file_A << std::endl;
    std::cout << file_B << std::endl;
    std::cout << file_C << std::endl;
    std::cout << std::endl;

    // file examination
    std::vector<std::string> file_vec = {file_A, file_B, file_C};
    for (auto &it: file_vec) {
        std::ofstream exam{it};
        assert(exam.is_open());
        exam.close();
    }

    int n = 30;
    int cache_size = 107;

    MatrixMultiplier<int> matrix_A(n, n, cache_size);
    MatrixMultiplier<int> matrix_B(n, n, cache_size);
    MatrixMultiplier<int> matrix_C(n, n, cache_size);

    std::vector<std::string> order_list{"ijk", "ikj", "jik",
                                        "jki", "kij", "kji"};

//    std::vector<std::string> order_list{"ikj"};

    std::cout << "it takes some time to travel all situation" << std::endl;
    std::cout <<std::endl;

    for (const auto &it0: order_list) {
        std::vector<Sequence> mul_order = Order::getOrder(it0);

        auto start_time = std::chrono::system_clock::now();

        matrix_A.multiply(matrix_B, matrix_C, mul_order, file_A, file_B, file_C);

        auto end_time = std::chrono::system_clock::now();

        std::cout << "multiply order ";
        for (auto it: mul_order) {
            std::cout << it << " ";
        }
        std::cout << std::endl;

        std::cout << "time spent "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()
                  << " ms" << std::endl;

        std::cout << "Matrix A" << std::endl;
        matrix_A.printInfo();

        std::cout << std::endl;

        std::cout << "Matrix B" << std::endl;
        matrix_B.printInfo();

        std::cout << std::endl;

        std::cout << "Matrix C" << std::endl;
        matrix_C.printInfo();

        std::cout << std::endl;
    }

    return 0;
}
