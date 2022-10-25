#pragma once

#include "Buffer.hpp"
#include <string>
#include <random>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <iostream>

template<typename T>
constexpr bool is_int() {
    return false;
}

template<>
constexpr bool is_int<int>() {
    return true;
}

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

    std::vector<long long> defaultGenSeq(const std::string &in_file_loc, const std::string &out_file_loc,
                                         int main_memo_size,
                                         long long &file_size) {
        std::vector<long long> seq_p;
        Buffer<T> mainMem(main_memo_size);
        long long read_p = 0;
        long long write_p = 0;
        std::string fin = in_file_loc;
        std::string fout = out_file_loc;
        seq_p.push_back(std::ios::beg);
        std::cout << "mainMemo:" << std::endl;
        while (read_p != file_size) {
            long long read_bytes = std::min((long long) mainMem.size() * sizeof(T),
                                            (unsigned long long) file_size - read_p);
            mainMem.read_n(fin, read_p, read_bytes);
            mainMem.ascendSort();
            for (int i = 0; i < mainMem.size() - mainMem.getInvalidNum(); ++i) { std::cout << mainMem[i] << " "; }
            mainMem.write_n(fout, write_p, read_bytes);
            seq_p.push_back(write_p);
        }
        std::cout << std::endl;
        return seq_p;
    }

    void loserTreeGenSeq(const std::string &in_file_loc,
                         const std::string &out_file_loc,
                         Buffer<T> mainMem,
                         int sequence_num) {

    }
};