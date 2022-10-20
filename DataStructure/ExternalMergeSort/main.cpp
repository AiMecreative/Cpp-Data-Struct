#include <iostream>
#include <vector>

int main() {
    std::vector<int> demo;
    demo.assign(100, 0);
    std::cout << demo.size();
    demo.push_back(99);
    std::cout << demo.back();
    return 0;
}
