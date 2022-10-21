#pragma once

#include "Buffer.h"
#include "LoserTree.h"
#include <vector>
#include <limits>

namespace SortFunc {

    template<typename T>
    static T empty_flag_ = std::numeric_limits<T>::max(); // used for empty sequence, the last value

    // vector_values only includes the values that should be compared in this run

    // compare with select
    // values vector stored the first value for every buffer
    template<typename T>
    int defaultCompare(int buf_loc, Buffer<T> &cmp_buf, std::vector<T> values);

    template<typename T>
    void defaultCompareWrapper(Buffer<T> &cmp_buf, std::vector<Buffer<T> > &input_buf);

    // compare with loser tree
    // where tree_buf is the constraint of loser tree
    template<typename T>
    void loserTree(int buf_loc, Buffer<T> &tree_buf, std::vector<T> values);
}