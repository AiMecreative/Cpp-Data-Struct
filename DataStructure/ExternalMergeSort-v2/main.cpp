#include "SorterWrapper.hpp"

int main() {
    setbuf(stdout, 0);

    const std::string file_A = DATA_DIR  "/data/file_A.txt";
    const std::string file_B = DATA_DIR  "/data/file_B.txt";

    /*
     * Attention:
     *
     * In "default" mode, ONLY 2-way merge is provided:
     * Main memory size should the times of (2 * merge_num) if using "default" mode
     * - compare buffer size = main memory size / 2
     * - input buffer size = compare buffer size / merge_num
     */

    SorterWrapper<float> sorter;
    sorter.data_size_ = 512;
    sorter.input_buf_size_ = 16;
    sorter.main_memo_size_ = 64;
    sorter.leaf_size_ = 16;
    sorter.gen_in_size_ = 16;
    sorter.gen_out_size_ = 16;

    /*
     * 2-way default merge sort with default generator
     */
    sorter.merge(file_A, file_B, "default");

    /*
     * 2-way default merge sort with loser tree generator
     */
    sorter.merge(file_A, file_B, "loser tree 2-way");

    /*
     * k-way loser tree merge sort with loser tree generator
     */
    sorter.merge(file_A, file_B, "loser tree k-way");

    return 0;
}
