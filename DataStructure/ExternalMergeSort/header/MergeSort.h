#pragma once

#include "Generator.h"
#include "SortFunc.h"
#include <string>

template<typename T>
class MergeSort {
    // MainMemoSize is k times of input buffer
private:
    int data_size_;

    int main_memo_size_;
    Buffer<T> main_memo_;

    int input_buf_size_;
    int middle_buf_size_;
    int output_buf_size_;

    int sequence_num_;
    std::vector<long long> seq_p_;

    long long file_size;

public:
    MergeSort() {
        main_memo_size_ = 0;
        input_buf_size_ = 0;
        middle_buf_size_ = 0;
        output_buf_size_ = 0;
        sequence_num_ = 0;
        data_size_ = 0;
        file_size = 0;
    }

    MergeSort(int main_memo_size, int sequence_num, int data_size, const std::string &mode) {
        main_memo_size_ = main_memo_size;
        sequence_num_ = sequence_num;
        data_size_ = data_size;
        if (mode == "default") {
            middle_buf_size_ = main_memo_size_ / 2;
            input_buf_size_ = middle_buf_size_ / sequence_num_;
        } else if (mode == "loser tree") {
            middle_buf_size_ = (int) main_memo_size_ * 0.4;
            input_buf_size_ = (int) main_memo_size_ * 0.4;
            output_buf_size_ = (int) main_memo_size_ * 0.2;
        } else {
            throw std::runtime_error(R"(define the mode: "default" | "loser tree" !")");
        }

        main_memo_.init(main_memo_size_);
        file_size = 0;
    }

    ~MergeSort() = default;

    // generate merge sequences
    void genSequences(const std::string &in_file, const std::string &out_file) {
        Generator<T> generator;
        generator.defaultGenSeq(in_file, out_file, main_memo_,
                                sequence_num_, seq_p_, file_size);
    }

    // generate random values
    void genRandomValue(const std::string &out_file, int bottom, int top) {
        Generator<T> generator;
        file_size = generator.genRandomValue(out_file, data_size_, bottom, top);
    }

    typedef int(*sortFunc)(Buffer<T> &cmp_buf, std::vector<Buffer<T> > &input_buf);

    // merge sequences from target file
    void merge(const std::string &in_file, const std::string &out_file, sortFunc sort_func) {
        // generate buffer vector
        std::vector<Buffer<T> > buf_vec;
        for (int i = 0; i < sequence_num_; ++i) { buf_vec.push_back(Buffer<T>(input_buf_size_)); }
        Buffer<T> cmp_buf(middle_buf_size_);

        std::vector<long long> read_pointers = seq_p_;
        read_pointers.pop_back();
        std::vector<long long> next_read_p;
        next_read_p.push_back(std::ios::beg);
        long long write_p = 0;

        while (read_pointers.size() > 1) {
            int seq_ind = 0;
            for (auto &buf: buf_vec) {

                // the size of every sequence is equal to the MainMemo size
                // MainMemoSize is k times of input buffer
                buf.readFill(in_file, read_pointers.at(seq_ind), file_size);

                seq_ind += 1;
            }

            // update when finished the one run
            if (read_pointers.back() == file_size) {
                std::swap(read_pointers, next_read_p);
                next_read_p.clear();
                next_read_p.push_back(std::ios::beg);
            }

            // finish one k-merge sort
            sort_func(cmp_buf, buf_vec);

            std::cout << "sorted:" << std::endl;
            for (int ind = 0; ind < cmp_buf.size(); ++ind) {
                std::cout << cmp_buf[ind] << " ";
            }
            std::cout << std::endl;

            // write the cmp_buf
            cmp_buf.writeAll(out_file, write_p, file_size);
            next_read_p.push_back(write_p);

        }
    }

    void defaultMerge(const std::string &in_file, const std::string &out_file) {
        merge(in_file, out_file, (sortFunc) SortFunc::defaultCompareWrapper<T>);
    }


};