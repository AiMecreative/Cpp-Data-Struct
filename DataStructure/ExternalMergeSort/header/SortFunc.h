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
    int defaultCompare(int buf_loc, Buffer<T> &cmp_buf, std::vector<T> values) {
        assert(!cmp_buf.empty() && !values.empty());
        int min_ind = 0;
        for (int ind = 0; ind < values.size(); ++ind) {
            if (values.at(min_ind) > values.at(ind)) {
                min_ind = ind;
            }
        }
        cmp_buf[buf_loc] = values.at(min_ind);
        return min_ind; // which input buffer
    }

    template<typename T>
    void defaultCompareWrapper(Buffer<T> &cmp_buf, std::vector<Buffer<T> > &input_buf) {
        assert(!cmp_buf.empty() && !input_buf.empty());

        std::vector<T> front_values;

        std::vector<int> front_ind(input_buf.size(), 0);

        // front vector initialization
        for (int buf = 0; buf < input_buf.size(); ++buf) {
            front_values.push_back(input_buf[buf][front_ind[buf]]);
        }

        // record the middle buffer index
        int mid_buf_loc = 0;

        // record: the total number of empty sequences
        int empty_num = 0;

        while (empty_num < input_buf.size()) {

            // which input buffer
            int buf_ind = defaultCompare(mid_buf_loc, cmp_buf, front_values);

            front_ind[buf_ind] += 1;

            if (front_ind[buf_ind] == input_buf[buf_ind].size()) {
                front_ind[buf_ind] -= 1;
                input_buf.at(buf_ind)[front_ind[buf_ind]]= empty_flag_<T>;
                empty_num += 1;
            }

            front_values[buf_ind] = input_buf[buf_ind][front_ind[buf_ind]];

            mid_buf_loc += 1;
        }
    }

    // compare with loser tree
    // where tree_buf is the constraint of loser tree
    template<typename T>
    void loserTree(int buf_loc, Buffer<T> &tree_buf, std::vector<T> values);
}