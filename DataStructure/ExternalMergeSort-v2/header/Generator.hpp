#pragma once

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
     * ATTENTION:
     * inorder to generate merge sequences as little as possible, we define:
     * output_size > leaf_size > input_size
     */

    void fillLoserTree(int start_idx, Buffer<T> &input_buf, LoserTree<T> &lt, int fill_num = 0) {
        for (int i = start_idx; i < lt.getLeafSize() - fill_num; ++i) {
            lt[i] = input_buf[i - start_idx];
        }
        for (int i = lt.getLeafSize() - 1; i >= lt.getLeafSize() - fill_num; --i) {
            lt.setEmpty(i);
        }
    }

    void popAll(int start_idx, Buffer<T> &output_buf, LoserTree<T> &lt, int fill_num = 0) {
        for (int i = start_idx; i < output_buf.size() - fill_num; ++i) {
            output_buf[i] = lt[i - start_idx];
        }
        output_buf.tagEmpty(output_buf.size() - 1 - fill_num);
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

            std::cout << std::endl;
            std::cout << "****" << idx << std::endl;

            std::unique_lock in_lock(in_mutex);
            std::cout << idx << " read" << std::endl;
            input_pipe[idx].read_n(file_in, read_p, read_bytes);
            for (int i = 0; i < input_size; ++i) {
                std::cout << "read buf: " << input_pipe[idx][i] << std::endl;
            }
            in_lock.unlock();

            std::unique_lock lt_lock(lt_mutex);
            std::cout << idx << " lt" << std::endl;
            lt.loadData(input_pipe[idx]);
            lt.popAll(output_pipe[idx]);
            lt_lock.unlock();

            std::unique_lock out_lock(out_mutex);
            std::cout << idx << " write" << std::endl;
            for (int i = 0; i < output_size; ++i) {
                std::cout << "out buf: " << output_pipe[idx][i] << std::endl;
            }
            output_pipe[idx].write_n(file_out, write_p, write_bytes);
            seq_p.push_back(write_p);
            out_lock.unlock();
        };

        while (write_p < file_size) {
            std::thread io00(io, 0);
            std::cout << std::endl;
            std::cout << "io00 id: " << io00.get_id() << std::endl;
            io00.join();
            std::thread io11(io, 1);
            std::cout << std::endl;
            std::cout << "io11 id: " << io11.get_id() << std::endl;
            io11.join();
        }

        std::cout << "seq_p in loser tree generate sequences:" << std::endl;
        for (auto &value: seq_p) {
            std::cout << value << " ";
        }
        std::cout << seq_p.size() << std::endl;
        return seq_p;
    }
};