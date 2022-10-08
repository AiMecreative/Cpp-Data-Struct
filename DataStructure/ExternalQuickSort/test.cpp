#include "ExternalQS.h"

int main(int argc, char **argv) {
    ExternalQS<int> sortOp; //sort operator
    int data_size = DATA_SIZE;
    const std::string rand_type = "int";

    sortOp.generate_rand(rand_type, 0, 100, data_size);
    sortOp.sort();
    sortOp.show(0, 100);

    return 0;
}