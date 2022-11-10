#include "MergeSort.hpp"
#include "LoserTree.hpp"
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    setbuf(stdout, 0);
    /*
     * file define and clear
     */
    fs::path path = fs::current_path().parent_path();
    const std::string cur_path = path.u8string();

    const std::string file_A = DATA_DIR  "/data/file_A.txt";
    const std::string file_B = DATA_DIR  "/data/file_B.txt";

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

    /*
     * 2-way default merge sort test
     */

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

    // input buffer size = 16
    MergeSort<int> sorter(512, "loser tree k-way", 32);
    sorter.genTestValues(file_A, 1, 10);
    // generate sequences with 32 length, 16 in total.
    // so the output buffer size is 16, leaf size is 16
    sorter.loserTreeGenMergeSeq(file_A, file_A, 32,  32, 32);
    sorter.loserTreeMerge(file_A, file_B);
    sorter.printSortedValues();
    return 0;
}
