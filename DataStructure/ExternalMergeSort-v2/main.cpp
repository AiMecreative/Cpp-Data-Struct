#include "MergeSort.hpp"
#include "LoserTree.hpp"
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    fs::path path = fs::current_path().parent_path();
    const std::string cur_path = path.u8string();

    const std::string file_A = cur_path + R"(\data\file_A.txt)";
    const std::string file_B = cur_path + R"(\data\file_B.txt)";

    // clear file content
    std::ifstream source_demo{file_A};
    std::ifstream result_demo{file_B};
    assert(source_demo.is_open() && result_demo.is_open());
    source_demo.close();
    result_demo.close();

    /*
     * Attention:
     *
     * In "default" mode, ONLY 2-way merge is provided:
     * Main memory size should the times of (2 * merge_num) if using "default" mode
     * - compare buffer size = main memory size / 2
     * - input buffer size = compare buffer size / merge_num
     */

    // 6, 35, 3
    // merge_num < sequences_num: endless loop
//    int main_size = 256;
//    int data_size = 12441;
//    int merge_num = 2;
//    const std::string mode = "default";
//    MergeSort<int> sorter(main_size, data_size, merge_num, mode);
//    sorter.bufferInitial();
//    sorter.generateTestValues(file_A, 1, 10);
//    sorter.generateMergeSeq(file_A, file_A);
//    sorter.defaultMerge(file_A, file_B);
//    sorter.printSortedValues();
    long long file_size = 20;
    Generator<int> generator;
    generator.genRandomValue(file_A, 8, 1, 10);
    generator.loserTreeGenSeq(file_A, file_B, 8, 8, 8, file_size);


    return 0;
}
