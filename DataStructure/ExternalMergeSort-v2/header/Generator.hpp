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
#include <atomic>
#include <condition_variable>


template<typename T>
constexpr bool is_int() {
    return false;
}

template<>
constexpr bool is_int<int>() {
    return true;
}

std::atomic<int> NEXT_INPUT_BUF = 0;
std::atomic<int> NEXT_OUTPUT_BUF = 0;
std::atomic<bool> INPUT_READY = false;
std::atomic<bool> OUTPUT_READY = false;
static int BUF_COUNT = 2;

template<typename T>
class Generator {
private:

public:
    Generator() = default;

    ~Generator() = default;

    /*
     * out_file_loc: output file
     * data_size:
     * bottom, top: generate data from bottom to top randomly
     * return: file size
     */
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


    /*
     * in_file_loc: from this file
     * out_file_loc: generate merge sequences to write to this file
     * main_memo_size: is equal to the sequences' length
     * file_size: the limit of file size
     * return: sequences' pointer in file
     */
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


    std::vector<T> genLeaves(const std::string &in_file_loc, long long &read_p, int leaf_size) {
        std::fstream read_file{in_file_loc, std::ios::in | std::ios::out | std::ios::binary};
        std::vector<T> leaves(leaf_size, 0);
        long long read_bytes = leaf_size * sizeof(T);
        read_file.seekg(read_p, std::ios::beg);
        read_file.read(reinterpret_cast<char *>(leaves.data()), read_bytes);
        read_p = read_file.tellg();
        read_file.close();
        return leaves;
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
        // recording the read/write location
        long long read_p = 0;
        long long write_p = 0;
        // the return vector
        std::vector<long long> seq_p;
        // input/output buffers and initial
        std::vector<Buffer<T> > input_buf;
        std::vector<Buffer<T> > output_buf;
        for (int _ = 0; _ < BUF_COUNT; ++_) {
            input_buf.push_back(Buffer<T>(input_size));
            output_buf.push_back(Buffer<T>(output_size));
        }
        // file location
        std::string file_in = in_file_loc;
        std::string file_out = out_file_loc;
        // initialize loser tree
        LoserTree<T> loserTree(genLeaves(file_in, read_p, leaf_size));
        loserTree.construct();

        // mutex and condition variables
        std::mutex read_mutex;
        std::mutex write_mutex;
        std::condition_variable read_cv;
        std::condition_variable write_cv;

        // thread functions define
        auto read_func = [&](long long start_p, long long read_bytes) {
            std::cout << "read_thread test" << std::endl;
            while (read_p < file_size) {
                std::cout << "read_loop test" << std::endl;
                std::unique_lock read_lock(read_mutex);
                std::cout << "read_lock test" << std::endl;
                read_cv.wait(read_lock);
                std::cout << "this input buf: " << NEXT_INPUT_BUF << std::endl;
                input_buf[NEXT_INPUT_BUF].read_n(file_in, start_p, read_bytes);
                NEXT_INPUT_BUF += 1;
                NEXT_INPUT_BUF = NEXT_INPUT_BUF % 2;
                read_bytes = 0;
            }
            std::cout << "read thread exit!" << std::endl;
        };

        auto write_func = [&](long long start_p, long long write_bytes) {
            std::cout << "write_thread test" << std::endl;
            while (write_p < file_size) {
                std::cout << "write_loop test" << std::endl;
                std::unique_lock write_lock(write_mutex);
                std::cout << "write_lock test" << std::endl;
                write_cv.wait(write_lock);
                std::cout << "this output buf: " << NEXT_OUTPUT_BUF << std::endl;
                output_buf[NEXT_OUTPUT_BUF].write_n(file_out, start_p, write_bytes);
                seq_p.push_back(start_p);
                NEXT_OUTPUT_BUF += 1;
                NEXT_OUTPUT_BUF = NEXT_OUTPUT_BUF % 2;
                write_bytes = 0;
            }
            std::cout << "write thread exit!" << std::endl;
            return true;
        };

        // create two threads
        long long read_bytes = 0;
        long long write_bytes = 0;
        std::thread read_thread(read_func, read_p, read_bytes);
        std::thread write_thread(write_func, write_p, write_bytes);

        int in_buf_idx = 0;
        int out_buf_idx = 0;
        while (write_p < file_size) {
            // write output buf when full
            std::cout << "in main thread: next_output_buf " << NEXT_OUTPUT_BUF
                      << " next_input_buf " << NEXT_INPUT_BUF << std::endl;
            if (output_buf[NEXT_OUTPUT_BUF].isFull()) {
                write_cv.notify_one();
            }
            // read input buf when free
            if (input_buf[NEXT_INPUT_BUF].isFree()) {
                read_cv.notify_one();
            }
            // sort for sequences
            int min_idx = loserTree.getMin();
            T min_value = loserTree[min_idx];
            T new_value = input_buf[NEXT_INPUT_BUF][in_buf_idx];
            in_buf_idx += 1;
            output_buf[NEXT_OUTPUT_BUF][out_buf_idx] = min_value;
            out_buf_idx += 1;
            loserTree.adjust(min_idx, new_value);
            read_bytes += sizeof(T);
            write_bytes += sizeof(T);
        }
        if (read_thread.joinable()) {
            read_thread.join();
        }
        if (write_thread.joinable()) {
            write_thread.join();
        }
        std::cout << "seq_p " << std::endl;
        for (auto &v: seq_p) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
        return seq_p;
    }
};