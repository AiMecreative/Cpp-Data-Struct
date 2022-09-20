#include "defs.h"
#include "PriorityDeque.h"
#include "ExternalQS.h"

using namespace std;

int main(int argc, char **argv) {
    ExternalQS<int> sortOp; //sort operator
    sortOp.read_txt("./data/source_data.txt");
    sortOp.sort();

    return 0;
}