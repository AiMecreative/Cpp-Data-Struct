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
                        const std::string &type, int bottom, int top) {
        std::ofstream out_file{out_file_loc, std::ios::binary | std::ios::out};
        assert(out_file.is_open() && out_file.good());
        static std::default_random_engine gen((unsigned int) time(nullptr));
        if (is_int<T>()) {
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
        std::fstream read_file{in_file_loc, std::ios::binary | std::ios::in};
        std::fstream write_file{out_file_loc, std::ios::binary | std::ios::out};
        assert(read_file.is_open() && read_file.good() &&
               write_file.is_open() && write_file.good());
        long long read_p = 0;
        long long write_p = 0;
        for (int seq = 0; seq < sequence_num; ++seq) {
            mainMem.readFill(read_file, read_p);
            mainMem.ascendSort();
            mainMem.writeAll(write_file, write_p);
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