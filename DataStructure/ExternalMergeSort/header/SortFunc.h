#pragma once

#include "Buffer.h"
#include "LoserTree.h"
#include <vector>

namespace SortFunc {

    // vector_values only includes the values that should be compared in this run

    // compare with select
    template<typename T>
    int defaultCompare(int buf_loc, Buffer<T> &cmp_buf, std::vector<T> values);

    // compare with loser tree
    // where tree_buf is the constraint of loser tree
    template<typename T>
    void loserTree(int buf_loc, Buffer<T> &tree_buf, std::vector<T> values);
}