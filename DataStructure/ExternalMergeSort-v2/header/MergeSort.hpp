#pragma once

#include "Buffer.hpp"
#include "Generator.hpp"
#include "SortFunc.hpp"
#include <cmath>
#include <iostream>

template<typename T>
class MergeSort {
private:

    int main_memo_size_;
    int input_buf_size_;
    int output_buf_size_;
    int compare_buf_size_;

    std::vector<Buffer<T> > input_buf_;
    Buffer<T> compare_buf_;

    int merge_num_;

    int data_size_;
    long long file_size_;

    int sequence_num_;

    // initialize when generate merge sequences the 1st time
    std::vector<long long> seq_start_p_;
    std::vector<long long> seq_end_p_;

    std::string sort_mode_{};

    std::string result_loc_{};

public:
    MergeSort() {
        main_memo_size_ = 0;
        input_buf_size_ = 0;
        output_buf_size_ = 0;
        compare_buf_size_ = 0;
        data_size_ = 0;
        file_size_ = 0;
        merge_num_ = 0;
        sequence_num_ = 0;
    }

    ~MergeSort() = default;


    MergeSort(int main_size, int data_size, int merge_num, const std::string &mode) {
        main_memo_size_ = main_size;
        data_size_ = data_size;
        merge_num_ = merge_num;
        sort_mode_ = mode;
        file_size_ = data_size_ * sizeof(T);

        sequence_num_ = std::ceil(data_size_ / (float) main_memo_size_);

        if (mode == "default") {
            compare_buf_size_ = main_memo_size_ / 2;
            input_buf_size_ = compare_buf_size_ / merge_num_;
        } else if (mode == "loser tree") {

        } else {
            throw std::runtime_error("error in MergeSort(), give `mode`: default | loser tree");
        }
    }


    void bufferInitial() {
        if (sort_mode_ == "default") {
            for (int i = 0; i < merge_num_; ++i) {
                input_buf_.push_back(Buffer<T>(input_buf_size_));
            }
            compare_buf_.init(compare_buf_size_);
        } else if (sort_mode_ == "loser tree") {

        } else {
            throw std::runtime_error("error in bufferInitial(), give `mode`: default | loser tree");
        }
    }

    void printSortedValues() {
        std::fstream result_file{result_loc_, std::ios::out | std::ios::in | std::ios::binary};
        assert(result_file.is_open() && result_file.good());
        std::vector<T> temp(data_size_, 0);
        result_file.seekp(0, std::ios::beg);
        result_file.read(reinterpret_cast<char*>(temp.data()), file_size_);
        std::cout << "sorted result: " << std::endl;
        for (auto &value: temp) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        result_file.close();
    }

    void generateTestValues(const std::string &in_file, int bottom, int top) {
        Generator<T> gen;
        file_size_ = gen.genRandomValue(in_file, data_size_, bottom, top);
    }

    void generateMergeSeq(const std::string &in_file, const std::string &out_file) {
        Generator<T> gen;
        seq_end_p_ = gen.defaultGenSeq(in_file, out_file, main_memo_size_, file_size_);
        seq_start_p_ = seq_end_p_;
        seq_start_p_.pop_back();
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
    void defaultMerge(const std::string &file_A, const std::string &file_B) {
        merge(file_A, file_B, (sortFunc) sortFunction::defaultCompareWrapper);
    }


    //
};