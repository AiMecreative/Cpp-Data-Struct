#include "defs.h"
#include "PriorityDeque.h"
#include "ExternalQS.h"
#include <deque>

int main(int argc, char **argv) {
    std::deque<int> qs_deque;
    ExternalQS<int> sortOp; //sort operator
    int *buffer = new int[MIDDLE_SIZE];
    int p_file = 0;
    int data_size = DATA_SIZE;
    const std::string file_loc = ASSET_DIR "/source_data.txt";
    const std::string rand_type = "int";

    sortOp.generate_rand(file_loc, rand_type, 0, 100, data_size);

    sortOp.qsDeque(qs_deque, data_size);

    return 0;
}