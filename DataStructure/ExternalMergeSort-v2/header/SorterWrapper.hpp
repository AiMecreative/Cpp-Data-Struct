#pragma once

#include "MergeSort.hpp"

#define RANDOM_MIN 1
#define RANDOM_MAX 1000


template<typename T>
class SorterWrapper {
public:

    int data_size_{};
    int input_buf_size_{};
    int main_memo_size_{};
    int leaf_size_{};
    int gen_in_size_{};
    int gen_out_size_{};

    void merge(const std::string &file_A, const std::string &file_B, const std::string &mode) {
        auto file_clear_ =
                [&](const std::string &file_A, const std::string &file_B
                ) {
                    // clear file content
                    std::ifstream source_demo{file_A};
                    std::ifstream result_demo{file_B};
                    assert(source_demo.is_open() && result_demo.is_open());
                    source_demo.close();
                    result_demo.close();
                };

        auto default2Merge_ =
                [&](const std::string &file_A, const std::string &file_B,
                    int data_size, int input_buf_size,
                    int main_memo_size
                ) {
                    file_clear_(file_A, file_B);
                    MergeSort<T> sorter(data_size, "default", input_buf_size);
                    sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                    sorter.defaultGenMergeSeq(file_A, file_A, main_memo_size);
                    sorter.defaultTwoWayMerge(file_A, file_B);
                    sorter.printSortedValues();
                };

        auto loserTree2Merge_ =
                [&](const std::string &file_A, const std::string &file_B,
                    int data_size, int input_buf_size,
                    int leaf_size, int gen_in_size, int gen_out_size
                ) {
                    file_clear_(file_A, file_B);
                    MergeSort<T> sorter(data_size, "loser tree 2-way", input_buf_size);
                    sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                    sorter.loserTreeGenMergeSeq(file_A, file_A, leaf_size, gen_in_size, gen_out_size);
                    sorter.defaultTwoWayMerge(file_A, file_B);
                    sorter.printSortedValues();
                };

        auto loserTreeKMerge_ =
                [&](const std::string &file_A, const std::string &file_B, // IO files
                    int data_size, int input_buf_size,                    // merge sort required
                    int leaf_size, int gen_in_size, int gen_out_size      // generator required
                ) {
                    file_clear_(file_A, file_B);
                    MergeSort<T> sorter(data_size, "loser tree k-way", input_buf_size);
                    sorter.genTestValues(file_A, RANDOM_MIN, RANDOM_MAX);
                    sorter.loserTreeGenMergeSeq(file_A, file_A, leaf_size, gen_in_size, gen_out_size);
                    sorter.loserTreeMerge(file_A, file_B);
                    sorter.printSortedValues();
                };

        if (mode == "default") {
            default2Merge_(file_A, file_B, data_size_, input_buf_size_, main_memo_size_);
            return;
        }
        if (mode == "loser tree 2-way") {
            loserTree2Merge_(file_A, file_B, data_size_, input_buf_size_, leaf_size_, gen_in_size_, gen_out_size_);
            return;
        }
        if (mode == "loser tree k-way") {
            loserTreeKMerge_(file_A, file_B, data_size_, input_buf_size_, leaf_size_, gen_in_size_, gen_out_size_);
            return;
        }
    }
};