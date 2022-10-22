#pragma once

#include "Buffer.h"
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
        static std::default_random_engine gen((unsigned int) time(nullptr));
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
        out_file.seekg(0, std::fstream::end);
        long long file_size = out_file.tellg();
        out_file.close();
        return file_size;
    }

    void defaultGenSeq(const std::string &in_file_loc, const std::string &out_file_loc,
                       Buffer<T> mainMem, int &sequence_num, std::vector<long long> &seq_p,
                       long long &file_size) {
        long long read_p = 0;
        long long write_p = 0;
        seq_p.push_back(std::ios::beg);
        while (read_p != file_size) {
            mainMem.readFill(in_file_loc, read_p, file_size);
            mainMem.ascendSort();
            mainMem.writeAll(out_file_loc, write_p, file_size);
            seq_p.push_back(write_p);
        }
    }

    void loserTreeGenSeq(const std::string &in_file_loc,
                         const std::string &out_file_loc,
                         Buffer<T> mainMem,
                         int sequence_num) {

    }
};