#include "MergeSort.hpp"
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    fs::path path = fs::current_path().parent_path();
    const std::string cur_path = path.u8string();

    const std::string file_A = cur_path + R"(\data\source_data.txt)";
    const std::string file_B = cur_path + R"(\data\sorted_data.txt)";

    // clear file content
    std::ifstream source_demo{file_A};
    std::ifstream result_demo{file_B};
    assert(source_demo.is_open() && result_demo.is_open());
    source_demo.close();
    result_demo.close();

    int main_size = 8;
    int data_size = 20;
    int merge_num = 2;
    const std::string mode = "default";
    MergeSort<int> sorter(main_size, data_size, merge_num, mode);
    sorter.bufferInitial();
    sorter.generateTestValues(file_A, 0, 10);
    sorter.generateMergeSeq(file_A, file_A);
    sorter.defaultMerge(file_A, file_B);
    return 0;
}
