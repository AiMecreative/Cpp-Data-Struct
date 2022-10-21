#include "SortFunc.h"
#include <cassert>


// need size judgement of cmp_buf
template<typename T>
int SortFunc::defaultCompare(int buf_loc, Buffer<T> &cmp_buf, std::vector<T> values) {
    assert(!cmp_buf.empty() && !values.empty());
    int min_ind = 0;
    for (auto ind: values) {
        if (values.at(min_ind) > values.at(ind)) {
            min_ind = ind;
        }
    }
    cmp_buf[buf_loc] = values.at(min_ind);
    return min_ind; // which input buffer
}

// input buffer is a pipeline of buffers
template<typename T>
void SortFunc::defaultCompareWrapper(Buffer<T> &cmp_buf, std::vector<Buffer<T> > &input_buf) {
    assert(!cmp_buf.empty() && !input_buf.empty());

    std::vector<T> front_values;

    // front vector initialization
    for (auto &buf : input_buf) {
        front_values.push_back(buf.back());
        buf.pop_back();
    }

    int buf_loc = 0;

    // record: the total number of empty sequences
    int empty_num = 0;

    while (empty_num < input_buf.size()) {
        int buf_ind = defaultCompare(buf_loc, cmp_buf, front_values);
        front_values.at(buf_loc) = input_buf.at(buf_loc).back();
        input_buf.at(buf_loc).pop_back();
        if (input_buf.at(buf_loc).empty()) {
            input_buf.at(buf_ind).push_back(empty_flag_<T>);
            empty_num += 1;
        }

        buf_loc += 1;
    }
}