#pragma once

#include "Buffer.h"
#include <string>
#include <random>
#include <fstream>
#include <cassert>
#include <algorithm>

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
public:
    Generator() = default;

    ~Generator() = default;

    void genRandomValue(const std::string &out_file_loc, int data_size,
                        int bottom, int top) {
        std::ofstream out_file{out_file_loc, std::ios::binary | std::ios::out};
        assert(out_file.is_open() && out_file.good());
        static std::default_random_engine gen((unsigned int) time(nullptr));
        if constexpr (static_cast<bool>(is_int<T>())) {
            static std::uniform_int_distribution<T> uniform(bottom, top);
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                out_file.write((const char *) &value, sizeof(T));
            }
        } else {
            static std::uniform_real_distribution<T> uniform(bottom, top);
            for (int _ = 0; _ < data_size; ++_) {
                T value = uniform(gen);
                out_file.write((const char *) &value, sizeof(T));
            }
        }
        out_file.close();
    }

    void defaultGenSeq(const std::string &in_file_loc, const std::string &out_file_loc,
                       Buffer<T> mainMem, int &sequence_num, std::vector<long long> &seq_p) {
        long long read_p = 0;
        long long write_p = 0;
        seq_p.push_back(std::ios::beg);
        for (int seq = 0; seq < sequence_num; ++seq) {
            mainMem.readFill(in_file_loc, read_p);
            mainMem.ascendSort();
            mainMem.writeAll(out_file_loc, write_p);
            seq_p.push_back(write_p);
            if (read_p == EOF) { break; } // cnm CLion
        }
    }

    void loserTreeGenSeq(const std::string &in_file_loc,
                         const std::string &out_file_loc,
                         Buffer<T> mainMem,
                         int sequence_num) {

    }
};