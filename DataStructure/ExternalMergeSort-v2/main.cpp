#include "MergeSort.hpp"
#include "LoserTree.hpp"

#define RANDOM_MIN 1
#define RANDOM_MAX 1000

int main() {
    setbuf(stdout, 0);

    const std::string file_A = DATA_DIR  "/data/file_A.txt";
    const std::string file_B = DATA_DIR  "/data/file_B.txt";

    auto file_clear =
            [&](const std::string &file_A, const std::string &file_B
            ) {
                // clear file content
                std::ifstream source_demo{file_A};
                std::ifstream result_demo{file_B};
                assert(source_demo.is_open() && result_demo.is_open());
                source_demo.close();
                result_demo.close();
            };

    auto default2Merge =
            [&](const std::string &file_A, const std::string &file_B,
                int data_size, int input_buf_size,
                int main_memo_size
            ) {
                file_clear(file_A, file_B);
                MergeSort<int> sorter(data_size, "default", input_buf_size);
                sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                sorter.defaultGenMergeSeq(file_A, file_A, main_memo_size);
                sorter.defaultTwoWayMerge(file_A, file_B);
                sorter.printSortedValues();
            };

    auto loserTree2Merge =
            [&](const std::string &file_A, const std::string &file_B,
                int data_size, int input_buf_size,
                int leaf_size, int gen_in_size, int gen_out_size
            ) {
                file_clear(file_A, file_B);
                MergeSort<int> sorter(data_size, "loser tree 2-way", input_buf_size);
                sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                sorter.loserTreeGenMergeSeq(file_A, file_A, leaf_size, gen_in_size, gen_out_size);
                sorter.defaultTwoWayMerge(file_A, file_B);
                sorter.printSortedValues();
            };

    auto loserTreeKMerge =
            [&](const std::string &file_A, const std::string &file_B, // IO files
                int data_size, int input_buf_size,                    // merge sort required
                int leaf_size, int gen_in_size, int gen_out_size      // generator required
            ) {
                file_clear(file_A, file_B);
                MergeSort<int> sorter(data_size, "loser tree k-way", input_buf_size);
                sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                sorter.loserTreeGenMergeSeq(file_A, file_A, leaf_size, gen_in_size, gen_out_size);
                sorter.loserTreeMerge(file_A, file_B);
                sorter.printSortedValues();
            };

    /*
     * Attention:
     *
     * In "default" mode, ONLY 2-way merge is provided:
     * Main memory size should the times of (2 * merge_num) if using "default" mode
     * - compare buffer size = main memory size / 2
     * - input buffer size = compare buffer size / merge_num
     */


    /*
     * 2-way default merge sort with default generator
     */
    default2Merge(file_A, file_B, 512, 16, 64);

    /*
     * 2-way default merge sort with loser tree generator
     */
    loserTree2Merge(file_A, file_B, 512, 16, 16, 16, 16);

    /*
     * k-way loser tree merge sort with loser tree generator
     */
    loserTreeKMerge(file_A, file_B, 512, 16, 16, 16, 16);

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

    return 0;
}
