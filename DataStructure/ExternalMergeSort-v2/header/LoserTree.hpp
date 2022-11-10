#pragma once

/*
 * LoserTree.hpp
 *
 * define loser tree data struct and interfaces
 * support initialization, construct, adjust, make empty
 */


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
    std::vector<int> tree_;

    // store the values
    std::vector<T> leaf_;
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
        if (used(parent) && leaf_[tree_[parent]] >= leaf_[undef_index]) {
            parent = empty_index_;
        } else if (used(parent) && leaf_[tree_[parent]] < leaf_[undef_index]) {
            std::swap(undef_index, tree_[parent]);
            parent = empty_index_;
        }
        return parent;
    }


public:
    LoserTree() {
        leaf_size_ = 0;
        leaf_start_ = 0;
        top_value_ = 0;
    }

    explicit LoserTree(int size) {
        if (size == 0) {
            throw std::runtime_error("empty values in LoserTree constructor!");
        } else {
            leaf_size_ = size;
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
            leaf_ = std::vector(size, empty_flag_);
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
            if (target != empty_index_) {
                tree_[target] = undef_index;
            } else {
                top_value_ = undef_index;
            }
        }
    }

    // replace the old_value in vector[index] with value
    void adjust(int index) {
        assert(index >= 0 && index < tree_.size());
        int target = findTargetNode(index + leaf_start_, index);
        if (target != empty_index_) {
            tree_[target] = index;
        } else {
            top_value_ = index;
        }
    }

    void setEmpty(int idx) {
        leaf_[idx] = empty_flag_;
    }

    void setLeafSize(int size) {
        leaf_size_ = size;
    }

    int getLeafSize() {
        return leaf_size_;
    }

    T getMin() {
        return top_value_;
    }

    void makeEmptyTree() {
        for (int i = 0; i < tree_.size() - leaf_size_; ++i) {
            tree_[i] = empty_index_;
        }
    }

    void makeEmptyLeaf() {
        for (int i = leaf_start_; i < tree_.size(); ++i) {
            leaf_[i - leaf_start_] = empty_flag_;
        }
    }

    bool empty() {
        for (auto &value: leaf_) {
            if (value != empty_flag_) {
                return false;
            }
        }
        return true;
    }

    void loadData(Buffer<T> &in_buf) {
        makeEmptyTree();
        makeEmptyLeaf();
        for (int i = 0; i < leaf_size_; ++i) {
            leaf_[i] = in_buf[i];
        }
        construct();
    }

    void popAll(Buffer<T> &out_buf) {
        for (int i = 0; i < leaf_size_; ++i) {
            out_buf[i] = leaf_[top_value_];
            leaf_[top_value_] = empty_flag_;
            adjust(top_value_);
        }
    }

    void printTree() {
        for (auto &ind: tree_) {
            std::cout << ind << " ";
        }
        std::cout << std::endl;
    }


};