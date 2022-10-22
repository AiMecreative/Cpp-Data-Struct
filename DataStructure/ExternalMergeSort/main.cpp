#include "MergeSort.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

/*
 * std::ifstream is("numbers.txt");
  std::istream_iterator<double> start(is), end;
  std::vector<double> numbers(start, end);
  std::cout << "Read " << numbers.size() << " numbers" << std::endl;

  // print the numbers to stdout
  std::cout << "numbers read in:\n";
  std::copy(numbers.begin(), numbers.end(),
            std::ostream_iterator<double>(std::cout, " "));
  std::cout << std::endl;
 */

int main() {
    fs::path path = fs::current_path().parent_path();
    const std::string cur_path = path.u8string();

    const std::string source_file = cur_path + R"(\data\source_data.txt)";
    const std::string result_file = cur_path + R"(\data\sorted_data.txt)";

    // clear file content
    std::ifstream source_demo{source_file};
    std::ifstream result_demo{result_file};
    assert(source_demo.is_open() && result_demo.is_open());
    source_demo.close();
    result_demo.close();

    int data_size = 20;

    // 8, 4, 2, 2
    MergeSort<int> sort_op(8, 2, data_size, "default");

    sort_op.genRandomValue(source_file, 0, 20);

    sort_op.genSequences(source_file, source_file);

    sort_op.defaultMerge(source_file, result_file);

    return 0;
}
