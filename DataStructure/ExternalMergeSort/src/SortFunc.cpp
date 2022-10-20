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