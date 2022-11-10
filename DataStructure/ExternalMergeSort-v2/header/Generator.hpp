#pragma once

/*
 * Generator.hpp
 *
 * define generators
 * support generate random test values, merge sequences with loser tree or not
 */

#include "Buffer.hpp"
#include "LoserTree.hpp"

#include <string>
#include <random>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <utility>

#include <thread>
#include <mutex>
#include <atomic>
#include <future>

template<typename T>
constexpr bool is_int() {
    return false;
}

template<>
constexpr bool is_int<int>() {
    return true;
}

// buffer number
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
            std::cout << std::endl << "source data:" << std::endl;
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                std::cout << value << " ";
                out_file.write((const char *) &value, sizeof(T));
            }
            std::cout << std::endl << std::endl;
        } else {
            static std::uniform_real_distribution<T> uniform(bottom, top);
            std::cout << std::endl << "source data:" << std::endl;
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                std::cout << value << " ";
                out_file.write((const char *) &value, sizeof(T));
            }
            std::cout << std::endl << std::endl;
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
        while (read_p != file_size) {
            long long read_bytes = std::min((long long) (mainMem.size() * sizeof(T)),
                                            (file_size - read_p));
            mainMem.read_n(fin, read_p, read_bytes);
            mainMem.ascendSort();
            mainMem.write_n(fout, write_p, read_bytes);
            seq_p.push_back(write_p);
        }
        std::cout << std::endl;
        return seq_p;
    }


    /*
     * use multi-thread generation
     *
     * in_file_loc:
     * out_file_loc:
     * leaf_size: the leaf size used to generate loser tree
     * input_size: input buffer size
     * output_size: output buffer size
     * file_size: limit
     * return: sequences' pointers in file
     */

    std::vector<long long> loserTreeGenSeq(const std::string &in_file_loc,
                                           const std::string &out_file_loc,
                                           int leaf_size,
                                           int input_size,
                                           int output_size,
                                           long long &file_size) {
        std::string file_in = in_file_loc;
        std::string file_out = out_file_loc;

        long long read_p = std::ios::beg;
        long long write_p = std::ios::beg;
        long long read_bytes = input_size * sizeof(T);
        long long write_bytes = output_size * sizeof(T);
        std::vector<long long> seq_p{write_p};

        // initialize loser tree
        LoserTree<T> lt(leaf_size);

        std::vector<Buffer<T> > input_pipe;
        std::vector<Buffer<T> > output_pipe;
        for (int i = 0; i < BUF_COUNT; ++i) {
            input_pipe.push_back(Buffer<T>(input_size));
            output_pipe.push_back(Buffer<T>(output_size));
        }

        // start the multi-thread
        std::mutex in_mutex;
        std::mutex out_mutex;
        std::mutex lt_mutex;

        auto io = [&](int idx) {

            std::unique_lock in_lock(in_mutex);
            input_pipe[idx].read_n(file_in, read_p, read_bytes);
            in_lock.unlock();

            std::unique_lock lt_lock(lt_mutex);
            lt.loadData(input_pipe[idx]);
            lt.popAll(output_pipe[idx]);
            lt_lock.unlock();

            std::unique_lock out_lock(out_mutex);
            output_pipe[idx].write_n(file_out, write_p, write_bytes);
            seq_p.push_back(write_p);
            out_lock.unlock();
        };

        while (write_p < file_size) {
            std::thread io00(io, 0);
            io00.join();
            std::thread io11(io, 1);
            io11.join();
        }

        return seq_p;
    }
};