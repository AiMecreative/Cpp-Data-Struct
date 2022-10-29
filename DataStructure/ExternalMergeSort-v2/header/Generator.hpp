#pragma once

#include "Buffer.hpp"
#include "LoserTree.hpp"

#include <string>
#include <random>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>


template<typename T>
constexpr bool is_int() {
    return false;
}

template<>
constexpr bool is_int<int>() {
    return true;
}

int NEXT_INPUT_BUF = 0;
int NEXT_OUTPUT_BUF = 0;

template<typename T>
class Generator {
private:

public:
    Generator() = default;

    ~Generator() = default;

    long long genRandomValue(const std::string &out_file_loc, int data_size,
                             int bottom, int top) {
        std::fstream out_file{out_file_loc, std::ios::binary | std::ios::in | std::ios::out};
        assert(out_file.is_open() && out_file.good());
//        static std::default_random_engine gen((unsigned int) time(nullptr));
        static std::default_random_engine gen((unsigned int) 0);
        if constexpr (static_cast<bool>(is_int<T>())) {
            static std::uniform_int_distribution<T> uniform(bottom, top);
            std::cout << "source data:" << std::endl;
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                std::cout << value << " ";
                out_file.write((const char *) &value, sizeof(T));
            }
            std::cout << std::endl;
        } else {
            static std::uniform_real_distribution<T> uniform(bottom, top);
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                out_file.write((const char *) &value, sizeof(T));
            }
        }
        long long file_size = data_size * sizeof(T);
        out_file.close();
        return file_size;
    }

    std::vector<long long> defaultGenSeq(const std::string &in_file_loc,
                                         const std::string &out_file_loc,
                                         int main_memo_size,
                                         long long &file_size) {
        long long read_p = 0;
        long long write_p = 0;
        std::string fin = in_file_loc;
        std::string fout = out_file_loc;
        std::vector<long long> seq_p;
        Buffer<T> mainMem(main_memo_size);
        seq_p.push_back(std::ios::beg);
        std::cout << "mainMemo:" << std::endl;
        while (read_p != file_size) {
            long long read_bytes = std::min((long long) (mainMem.size() * sizeof(T)),
                                            (file_size - read_p));
            mainMem.read_n(fin, read_p, read_bytes);
            mainMem.ascendSort();
            for (int i = 0; i < read_bytes / sizeof(T); ++i) { std::cout << mainMem[i] << " "; }
            mainMem.write_n(fout, write_p, read_bytes);
            seq_p.push_back(write_p);
        }
        std::cout << std::endl;
        return seq_p;
    }

    /*
     * the length of merge sequence is equal to the size of output_buf
     * the length of loser tree **leaves** is equal to leaf_size
     */
    std::vector<long long> loserTreeGenSeq(const std::string &in_file_loc,
                                           const std::string &out_file_loc,
                                           int leaf_size,
                                           int input_size,
                                           int output_size,
                                           long long &file_size) {
        long long read_p = std::ios::beg;   // shared variable, reading pointer
        long long write_p = std::ios::beg;
        long long read_bytes = 0;
        long long write_bytes = 0;
        std::string in_loc = in_file_loc;
        std::string out_loc = out_file_loc;
        std::vector<long long> seq_p;
        std::vector<T> leaf_vec(leaf_size);
        std::vector<Buffer<T> > input_buf;  // 2 input buf
        std::vector<Buffer<T> > output_buf; // 2 output buf
        for (auto _ = 0; _ < 2; ++_) {
            input_buf.push_back(Buffer<T>(input_size));
            output_buf.push_back(Buffer<T>(output_size));
        }

        std::mutex read_mutex;
        std::mutex write_mutex;
        std::condition_variable read_cv;
        std::condition_variable write_cv;

        // read in to fill the loser tree
        read_bytes = leaf_size * sizeof(T);
        std::fstream read_file{in_file_loc, std::ios::in | std::ios::out | std::ios::binary};
        assert(read_file.is_open() && read_file.good());
        read_file.seekg(read_p, std::ios::beg);
        read_file.read(reinterpret_cast<char *>(leaf_vec.data()), read_bytes);
        read_p = read_file.tellg();
        read_file.close();
        // initialize loser tree
        // guarantee leaf_vec is full
        LoserTree<T> loserTree(leaf_vec);
        loserTree.construct();

        // read in values with multi-thread


        auto read_func = [&](int buf_idx, long long read_bytes) {
            std::unique_lock read_lock(read_mutex);
            read_cv.wait(read_lock);
            input_buf[buf_idx].read_n(in_loc, read_p, read_bytes);
        };

        auto write_func = [&](int buf_idx, long long write_bytes) {
            std::unique_lock write_lock(write_mutex);
            write_cv.wait(write_lock);
            output_buf[buf_idx].write_n(out_loc, write_p, write_bytes);
            seq_p.push_back(write_p);
        };

        int findOutBuf = [&](std::vector<Buffer<T> > vec_buf) {
            int result = -1;
            for (int buf = 0; buf < vec_buf.size(); ++buf) {
                result = buf;
                for (int loc = 0; loc < vec_buf[buf].size(); ++buf) {
                    if (!vec_buf[buf].isFree(loc)) {
                        result = -1;
                        break;
                    }
                }
            }
            return result;
        };

        int read_idx = 0, write_idx = 0;
        read_bytes = std::min((file_size - read_p), (long long) (input_size * sizeof(T)));
        write_bytes = read_bytes;
        std::thread read_thread(read_func, read_idx, read_bytes);
        std::thread write_thread(write_func, write_idx, write_bytes);

        // 0. fill input 0
        read_bytes = input_size * sizeof(T);
        input_buf[0].read_n(in_loc, read_p, read_bytes);

        while (read_p != file_size) {
            // read data to input_buffer
            read_cv.notify_one();
            // which output buffer is free
            write_idx = findOutBuf(output_buf);
            assert(write_idx != -1);
            // sort to generate merge sequences
            for (int i = 0; i < output_buf[write_idx].size(); ++i) {
                int min_idx = loserTree.getMin();
                T min_value = loserTree[min_idx];
                output_buf[write_idx][i] = min_value;
                if ()
            }
            write_cv.notify_one();

        }

        read_thread.join();
        write_thread.join();
        // join
    }
};