#pragma once

#include <string>

template<typename T>
class MergeSort {
private:
    int sequence_num_;

public:
    MergeSort() {
        sequence_num_ = 0;
    }

    ~MergeSort() = default;

    // generate merge sequences
    void genSequences(const std::string &in_file, const std::string &out_file);

    // generate random values
    void genRamdomValue(const std::string &out_file, int bottom, int top);


};