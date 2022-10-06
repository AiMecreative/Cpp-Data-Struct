#include "defs.h"
#include "PriorityDeque.h"
#include "ExternalQS.h"
#include <deque>

struct node1 {
    float l;
    int r;

};

int main(int argc, char **argv) {
    std::deque<int> qs_deque;
    ExternalQS<int> sortOp; //sort operator
    int *buffer = new int[MIDDLE_SIZE];
    int p_file = 0;
    int data_size = DATA_SIZE;
    const std::string file_loc = ASSET_DIR "/source_data.txt";
    const std::string rand_type = "int";

//    sortOp.generate_rand(rand_type, 0, 100, data_size);
//    sortOp.sort();
    static std::default_random_engine gen;
    static std::uniform_int_distribution dis(0, 10);
    for (int i = 0; i < 5; ++i) {
        qs_deque.push_front(dis(gen));
        qs_deque.push_back(dis(gen));
    }
    std::cout << qs_deque.size() << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << qs_deque.at(i) << " ";
    }

    int init = 0;
    float b;
    b = *(float*)&init;

    std::cout << std::endl;
    std::cout << typeid(b).name() << b << typeid(init).name() << init;

    std::deque<node1> testQ;
    static std::uniform_real_distribution<float> dis2(0, 10);
    for (int i = 0; i < 6; ++i) {
        node1 newNode;
        newNode.l = dis2(gen);
        newNode.r = dis2(gen);
        testQ.push_front(newNode);
    }
    for (auto it = testQ.begin(); it != testQ.end(); ++it) {
        std:: cout << (*it).l << " ";
    }

    return 0;
}