#define MIDDLE_SIZE 1000
#define SMALL_SIZE 200
#define LARGE_SIZE 200
#define INPUT_SIZE 200

#include <cstring>

template<typename T>
struct Bundle
{
    char type[3]; // min | max
    T value;
};
