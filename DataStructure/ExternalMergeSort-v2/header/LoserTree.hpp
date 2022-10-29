#pragma once

#include "Buffer.hpp"
#include <limits>
#include <vector>
#include <exception>
#include <cassert>
#include <iostream>

template<typename T>
class LoserTree {
private:
    static constexpr T empty_flag_ = std::numeric_limits<T>::max();
    static constexpr int empty_index_ = -1;

    int leaf_size_;
    int leaf_start_;

    // store the indices, including the leaf vector
    std::vector<T> tree_;

    // store the values
    std::vector<int> leaf_;
    T top_value_;

    // judge if the node is used
    bool used(int index) {
        assert(index >= 0 && index <= tree_.size());
        if (tree_[index] != empty_index_) return true;
        else return false;
    }


    int findTargetNode(int tree_index, int &undef_index) {
        int parent = (tree_index - 1) / 2;
        while (parent != 0) {
            if (!used(parent)) {
                return parent;
            } else if (leaf_[tree_[parent]] > leaf_[undef_index]) {
                parent = (parent - 1) / 2;
            } else {
                std::swap(undef_index, tree_[parent]);
                parent = (parent - 1) / 2;
            }
        }
        // parent == 0
        if (used(parent) && leaf_[tree_[parent]] > leaf_[tree_[undef_index]]) {
            parent = -1;
        }
        return parent;
    }


public:
    LoserTree() {
        leaf_size_ = 0;
        leaf_start_ = 0;
        top_value_ = 0;
    }

    explicit LoserTree(std::vector<T> values) {
        if (values.empty()) {
            throw std::runtime_error("empty values in LoserTree constructor!");
        } else {
            leaf_size_ = values.size();
            int k = 1;
            int tree_size = 1;
            while (k < leaf_size_) {
                k *= 2;
                tree_size += k;
            }
            tree_.assign(tree_size, empty_index_);
            top_value_ = empty_index_;
            // initial leaf values
            leaf_start_ = tree_size - leaf_size_;
            leaf_ = values;
            for (int i = 0; i < leaf_size_; ++i) {
                tree_[leaf_start_ + i] = i;
            }
        }
    }

    ~LoserTree() = default;

    T &operator[](int ind) {
        return leaf_[ind];
    }

    const T &operator[](int ind) const {
        return leaf_[ind];
    }

    // construct a loser tree based on initialized tree leaf
    void construct() {
        for (int leaf_index = leaf_start_; leaf_index < tree_.size(); ++leaf_index) {
            int undef_index = leaf_index - leaf_start_;
            int target = findTargetNode(leaf_index, undef_index);
            if (target != -1) {
                tree_[target] = undef_index;
            } else {
                top_value_ = leaf_index;
            }
        }
    }

    // replace the old_value in vector[index] with value
    void adjust(int index, int value) {
        assert(index >= 0 && index < tree_.size());
        int target = findTargetNode(index + leaf_start_, index);
        if (target != -1) {
            tree_[target] = index;
        } else {
            top_value_ = index;
        }
    }


    T getMin() {
        return top_value_;
    }


    bool empty() {
        return tree_.empty();
    }


    void printTree() {
        for (auto &ind: tree_) {
            std::cout << ind << " ";
        }
        std::cout << std::endl;
    }


};