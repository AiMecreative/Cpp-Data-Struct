#pragma once

/*
 * SortFunc.hpp
 *
 * define namespace sortFunction
 * support default compare and loser tree compare
 */

#include "Buffer.hpp"
#include "LoserTree.hpp"
#include <mutex>

namespace sortFunction {

    template<typename T>
    void restWrite_n(std::string &out_file_loc, std::string &in_file_loc,
                     long long &start_bytes, long long &write_bytes) {
        std::fstream source_file{out_file_loc, std::ios::in | std::ios::out | std::ios::binary};
        std::fstream target_file{in_file_loc, std::ios::in | std::ios::out | std::ios::binary};
        assert(target_file.is_open() && source_file.is_open());
        std::vector<T> temp(write_bytes / sizeof(T));
        source_file.seekp(start_bytes, std::ios::beg);
        target_file.seekg(start_bytes, std::ios::beg);
        source_file.read(reinterpret_cast<char *>(temp.data()), write_bytes);
        target_file.write(reinterpret_cast<const char *>(temp.data()), write_bytes);
        start_bytes += write_bytes;
        source_file.close();
        target_file.close();
    }

    // 2-way merge
    template<typename T>
    int defaultCompare(int buf_loc, Buffer<T> &cmp_buf, std::vector<T> values) {
        assert(!cmp_buf.empty() && !values.empty());
        int min_ind = 0;
        for (int ind = 0; ind < values.size(); ++ind) {
            if (values.at(min_ind) > values.at(ind)) {
                min_ind = ind;
            }
        }
        cmp_buf[buf_loc] = values.at(min_ind);
        return min_ind; // which input buffer
    }

    // 2-way merge (ps: 这个函数最开始写复杂了, 不想改了)
    template<typename T>
    void defaultCompareWrapper(std::string &file_A, std::string &file_B,
                               std::vector<Buffer<T> > &input_buf_,
                               Buffer<T> &output_buf_,
                               std::vector<long long> &seq_start_p_,
                               std::vector<long long> &seq_end_p_,
                               int &sequence_num_) {

        long long input_buf_size_ = input_buf_[0].size();
        long long compare_buf_size_ = output_buf_.size();
        std::vector<long long> reading_p = seq_start_p_;

        int buf_head = 0;
        int buf_tail = 0;
        int buf_limit = sequence_num_;

        std::vector<long long> next_start_p;
        std::vector<long long> next_end_p;
        next_end_p.push_back(std::ios::beg);

        // choose 2 buffer to merge in order
        for (buf_tail = 1; buf_tail < buf_limit; buf_tail += 2) {
            buf_head = buf_tail - 1;
            for (int buf = buf_head; buf <= buf_tail; ++buf) {
                long long read_bytes = input_buf_size_ * sizeof(T);
                input_buf_[buf - buf_head].read_n(file_A, reading_p[buf], read_bytes);
            }

            std::vector<T> front_values;
            std::vector<T> front_indices(2, 0);

            for (int i = 0; i < 2; ++i) {
                int index = front_indices[i];
                front_values.push_back(input_buf_[i][index]);
            }
            int empty_seq = 0;
            int cmp_loc = 0;
            // record the write-pointer
            long long write_p = next_end_p.back();
            // traverse sequences chosen until to the end
            while (empty_seq != 2) {
                // next buffer need to be fill
                int next_ind = defaultCompare(cmp_loc, output_buf_, front_values);
                cmp_loc += 1;
                // if compare_buffer is full, write to file, update pointer
                if (cmp_loc == compare_buf_size_) {
                    long long write_bytes = std::min((long long) (compare_buf_size_ * sizeof(T)),
                                                     seq_end_p_.back() - write_p);
                    output_buf_.write_n(file_B, write_p, write_bytes);
                    // update compare buffer index
                    cmp_loc = 0;
                }
                int in_buf_ind = front_indices[next_ind];
                // buffer to the bottom
                if (in_buf_ind == input_buf_[next_ind].size() - 1) {
                    // sequences has released values corresponding the input buffer , go on
                    if (reading_p[buf_head + next_ind] < seq_end_p_[buf_head + next_ind + 1]) {
                        // legal area
                        long long read_bytes = std::min((long long) (input_buf_size_ * sizeof(T)),
                                                        seq_end_p_[buf_head + next_ind + 1] -
                                                        reading_p[buf_head + next_ind]);
                        input_buf_[next_ind].read_n(file_A, reading_p[buf_head + next_ind], read_bytes);
                        // set empty if illegal
                        input_buf_[next_ind].tagEmpty(read_bytes / sizeof(T));
                        front_indices[next_ind] = 0;
                        in_buf_ind = 0;
                    } else {
                        // tag empty if nothing released
                        input_buf_[next_ind].tagEmpty(0);
                        empty_seq += 1;
                        // sequences over
                        if (empty_seq == 2) {
                            next_end_p.push_back(write_p);
                            sequence_num_ = sequence_num_ - 1;
                        }
                    }
                } else {
                    // not to the bottom, buffer index+1
                    front_indices[next_ind] += 1;
                    in_buf_ind += 1;
                }
                // update front vector
                front_values[next_ind] = input_buf_[next_ind][in_buf_ind];
            }

        }

        // 已读的数据大于实际写入的数据, 说明缓存中还有未写入的数据
        if (reading_p.back() > next_end_p.back()) {
            long long write_bytes = reading_p.back() - next_end_p.back();
            output_buf_.write_n(file_B, next_end_p.back(), write_bytes);
        }

        // 剩余元素直接写入, 等待下一次归并, 更新下一次的指针
        if (next_end_p.back() < seq_end_p_.back()) {
            long long write_bytes = seq_end_p_.back() - next_end_p.back();
            long long start_bytes = next_end_p.back();
            // file_A > file_B
            restWrite_n<T>(file_A, file_B, start_bytes, write_bytes);
            next_end_p.push_back(start_bytes);
        }

        // 更新指针
        next_start_p = next_end_p;
        next_start_p.pop_back();

        seq_start_p_ = next_start_p;
        seq_end_p_ = next_end_p;

    }


    /*
     * typedef void(*sortFunc)(std::string &file_A, std::string &file_B,
                            std::vector<Buffer<T> > &input_buf_,
                            Buffer<T> &output_buf_,
                            std::vector<long long> &seq_start_p_,
                            std::vector<long long> &seq_end_p_,
                            int &sequence_num_);
     */

    // loser tree k-way merge
    template<typename T>
    void loserTreeCompare(std::string &file_A, std::string &file_B,
                          std::vector<Buffer<T> > &input_pipe,
                          Buffer<T> &output_buf,
                          std::vector<long long> &seq_start_p,
                          std::vector<long long> &seq_end_p,
                          int &sequence_num) {
        std::vector<long long> reading_p = seq_start_p;
        int input_num = input_pipe.size();
        int input_size = input_pipe[0].size();
        int output_size = output_buf.size();
        long long read_bytes = input_size * sizeof(T);
        long long write_bytes = input_pipe.size() * sizeof(T);
        long long file_size = seq_end_p.back();

        long long write_p = std::ios::beg;

        // initialize loser tree, the length of leaf is equal to sequence number
        LoserTree<T> lt(sequence_num);

        // record which input buffer needs to read new value or fill max value
        // 0: need to read, 1: normal, -1: fill with empty
        std::vector<int> input_state(input_num, 0);

        // record pointers in input buffers
        std::vector<int> input_idx(input_num, 0);

        auto update_input = [&](int idx) {
            if (input_state[idx] == 0) {
                read_bytes = std::min(seq_end_p[idx + 1] - reading_p[idx], (long long) (input_size * sizeof(T)));
                input_pipe[idx].read_n(file_A, reading_p[idx], read_bytes);
                input_state[idx] = 1;
            } else if (input_state[idx] == -1) {
                input_pipe[idx].tagEmpty(0);
                input_state[idx] = 1;
            }
        };

        auto update_lt = [&](T &pop_value, int &pop_idx) {
            // get pop value and input buffer index
            pop_idx = lt.getMin();
            pop_value = lt[pop_idx];
            input_idx[pop_idx] += 1;
            if (input_idx[pop_idx] == input_size) {
                input_state[pop_idx] = 0;
                input_idx[pop_idx] = 0;
                if (seq_end_p[pop_idx + 1] - reading_p[pop_idx] < input_size * sizeof(T)) {
                    input_state[pop_idx] = -1;
                }
            }
            update_input(pop_idx);
            // update loser tree
            int in_input_idx = input_idx[pop_idx];
            lt[pop_idx] = input_pipe[pop_idx][in_input_idx];
            lt.adjust(pop_idx);
        };

        auto update_output = [&](int &output_idx, T value) {
            output_buf[output_idx] = value;
            output_idx += 1;
            if (output_idx == output_size) {
                write_bytes = std::min((long long) (output_size * sizeof(T)), file_size - write_p);
                output_buf.write_n(file_B, write_p, write_bytes);
                output_idx = 0;
            }

        };

        auto init_lt = [&]() {
            Buffer<T> temp(input_num);
            for (int i = 0; i < input_num; ++i) {
                temp[i] = input_pipe[i][0];
            }
            lt.loadData(temp);
        };

        auto init_input = [&]() {
            for (int i = 0; i < input_num; ++i) {
                update_input(i);
            }
        };

        std::once_flag init_lt_once;
        std::once_flag init_in_once;
        int output_idx = 0;
        // loop when there are values haven't been written
        while (write_p < file_size) {

            // init lt and input_pipe, only execute once
            std::call_once(init_in_once, init_input);
            std::call_once(init_lt_once, init_lt);

            // update loser tree
            T pop_value = 0;
            int pop_idx = 0;
            update_lt(pop_value, pop_idx);

            // write to output buffer and write to file if needed
            update_output(output_idx, pop_value);
        }

        sequence_num = 1;
    }

}