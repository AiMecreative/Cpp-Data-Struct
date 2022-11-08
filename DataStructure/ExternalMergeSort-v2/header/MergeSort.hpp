#pragma once

#include "Buffer.hpp"
#include "Generator.hpp"
#include "SortFunc.hpp"
#include <cmath>
#include <iostream>
#include <limits>

template<typename T>
class MergeSort {
private:
    /*
     * the parameters used in generating values or sequences is defined as local variables in relevant functions
     * such as: main_memo_size, gen_leaf_size. gen_input_size, gen_output_size
     *
     * there are only parameters that used in sorting
     */

    static constexpr T empty_flag_ = std::numeric_limits<T>::max();

    // 2-way default merge required:
    int input_buf_size_;
    int compare_buf_size_;
    Buffer<T> compare_buf_;
    std::vector<Buffer<T> > input_buf_;

    // 2-way loser tree merge required:
    int leaf_size_;
    int output_buf_size_;
    std::vector<T> leaf_;
    std::vector<Buffer<T> > output_buf_;

    // loser tree k-way merge sort required:
    //TODO: ep5 k-way merge

    // total data information
    int data_size_;
    int merge_num_;
    long long file_size_;
    int sequence_num_;

    // initialize when generate merge sequences the 1st time
    std::vector<long long> seq_start_p_;
    std::vector<long long> seq_end_p_;

    std::string sort_mode_{};
    std::string result_loc_{};

public:
    MergeSort() {
        input_buf_size_ = 0;
        compare_buf_size_ = 0;

        leaf_size_ = 0;
        output_buf_size_ = 0;

        data_size_ = 0;
        file_size_ = 0;
        merge_num_ = 0;
        sequence_num_ = 0;
    }

    ~MergeSort() = default;


    MergeSort(int data_size, const std::string &mode,
              int input_buf_size = 8, int compare_buf_size = 16,
              int output_buf_size = 16, int leaf_size = 16, int merge_num = 2) {

        data_size_ = data_size;
        sort_mode_ = mode;
        file_size_ = data_size_ * sizeof(T);

        sequence_num_ = seq_start_p_.size();

        if (mode == "default") {
            // input_buf_size, compare_buf_size required
            merge_num_ = 2;
            input_buf_size_ = input_buf_size;
            compare_buf_size_ = compare_buf_size;
        } else if (mode == "loser tree 2-way") {
            // input_buf_size, output_buf_size, leaf_size required
            merge_num_ = 2;
            input_buf_size_ = input_buf_size;
            output_buf_size_ = output_buf_size;
            leaf_size_ = leaf_size;
        } else if (mode == "loser tree k-way") {
            // input_buf_size, output_buf_size, leaf_size, merge_num required
            merge_num_ = merge_num;
            input_buf_size_ = input_buf_size;
            output_buf_size_ = output_buf_size;
            leaf_size_ = leaf_size;
        } else {
            throw std::runtime_error(
                    "error in MergeSort(), give `mode`: default | loser tree 2-way | loser tree k-way");
        }
    }


    // before sorting
    void initialize() {
        if (sort_mode_ == "default") {
            for (int i = 0; i < merge_num_; ++i) {
                input_buf_.push_back(Buffer<T>(input_buf_size_));
            }
            compare_buf_.init(compare_buf_size_);
        } else if (sort_mode_ == "loser tree 2-way") {
            for (int i = 0; i < merge_num_; ++i) {
                input_buf_.push_back(Buffer<T>(input_buf_size_));
                output_buf_.push_back(Buffer<T>(output_buf_size_));
            }
            leaf_.assign(leaf_size_, empty_flag_);
        } else if (sort_mode_ == "loser tree k-way") {
            for (int i = 0; i < merge_num_; ++i) {
                //TODO:
            }
        } else {
            throw std::runtime_error("error in bufferInitial(), give `mode`: default | loser tree");
        }
    }

    // after sorting
    void printSortedValues() {
        std::fstream result_file{result_loc_, std::ios::out | std::ios::in | std::ios::binary};
        assert(result_file.is_open() && result_file.good());
        std::vector<T> temp(data_size_, 0);
        result_file.seekp(0, std::ios::beg);
        result_file.read(reinterpret_cast<char *>(temp.data()), file_size_);
        std::cout << "sorted result: " << std::endl;
        for (auto &value: temp) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        result_file.close();
    }

    // before sorting, generate test values from bottom to top
    void genTestValues(const std::string &in_file, int bottom, int top) {
        Generator<T> gen;
        long long file_size;
        file_size = gen.genRandomValue(in_file, data_size_, bottom, top);
        if (file_size != file_size_) {
            throw std::runtime_error("different file_size in genTestValues()");
        }
    }

    // use main memory size to divide file into sequences
    void defaultGenMergeSeq(const std::string &in_file_loc, const std::string &out_file_loc, int main_memo_size) {
        Generator<T> gen;
        seq_end_p_ = gen.defaultGenSeq(in_file_loc, out_file_loc, main_memo_size, file_size_);
        seq_start_p_ = seq_end_p_;
        seq_start_p_.pop_back();
        sequence_num_ = seq_start_p_.size();
    }

    // use loser tree to divide sequences, length = gen_out_size_
    void loserTreeGenMergeSeq(const std::string &in_file_loc, const std::string &out_file_loc,
                              int leaf_size, int gen_in_size, int gen_out_size) {
        Generator<T> gen;
        seq_end_p_ = gen.loserTreeGenSeq(in_file_loc, out_file_loc, leaf_size,
                                         gen_in_size, gen_out_size, file_size_);
        seq_start_p_ = seq_end_p_;
        seq_start_p_.pop_back();
        sequence_num_ = seq_start_p_.size();
    }


    typedef void(*sortFunc)(std::string &file_A, std::string &file_B,
                            std::vector<Buffer<T> > &input_buf_,
                            int input_buf_size_,
                            Buffer<T> &compare_buf_,
                            int compare_buf_size_,
                            std::vector<long long> &seq_start_p_,
                            std::vector<long long> &seq_end_p_,
                            int &sequence_num_,
                            int merge_num_);

    void merge(const std::string &file_A, const std::string &file_B, sortFunc sort_func) {
        std::string file1 = file_A;
        std::string file2 = file_B;
        int swap_num = 0;

        // 当归并段数目为 1 时结束循环
        while (sequence_num_ != 1) {

            // 进行一轮归并
            sort_func(file1, file2,
                      input_buf_,
                      input_buf_size_,
                      compare_buf_,
                      compare_buf_size_,
                      seq_start_p_,
                      seq_end_p_,
                      sequence_num_,
                      merge_num_);


            std::string temp = file1;
            file1 = file2;
            file2 = temp;
            swap_num += 1;

        }

        if (swap_num % 2 == 0) {
            std::cout << "result in file " << file_A << std::endl;
            result_loc_ = file_A;
        } else {
            std::cout << "result in file " << file_B << std::endl;
            result_loc_ = file_B;
        }
    }

    // 2-way merge
    void defaultTwoWayMerge(const std::string &file_A, const std::string &file_B) {
        merge(file_A, file_B, (sortFunc) sortFunction::defaultCompareWrapper);
    }


    //
};