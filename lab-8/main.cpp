#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <iostream>

using namespace std;

void fillForward(int* array, size_t size);
void fillBackward(int* array, size_t size);
void fillRandom(int* array, size_t size);

void warmUp(const int* array, size_t size);

uint64_t getTick();
uint64_t traverseTicks(const int* array, size_t size);
uint64_t minTraverseTime(const int* array, size_t size);

int main() {
    const int MIN = 1024 / sizeof(int);
    const int MAX = 64*1024*1024 / sizeof(int);

    ofstream out("out.csv");
    out << "N,forward,backward,random\n";

    int* array = new int[MAX];

    size_t step = 1;

    for (size_t n = MIN; n < MAX; n += step) {
        cout << (n / (double)MAX * 100.)  << "%" << endl;

        out << (n * 4. / 1024) << ",";

        fillForward(array, n);
        warmUp(array, n);
        out << (minTraverseTime(array, n)) << ",";
        
        fillBackward(array, n);
        warmUp(array, n);
        out << (minTraverseTime(array, n)) << ",";

        fillRandom(array, n);
        warmUp(array, n);
        out << (minTraverseTime(array, n)) << "\n";

        step = 1. * n * n / MAX /4 + 1;
    }

    delete array;
    return 0;
}

void fillForward(int* array, size_t size) {
    array[--size] = 0;
    for (size_t i = 0; i < size; ++i) {
        array[i] = i + 1;
    }
}

void fillBackward(int* array, size_t size) {
    array[0] = --size;
    for (size_t i = size; i > 0; --i) {
        array[i] = i-1;
    }
}

void fillRandom(int* array, size_t size) {
    fillForward(array,size);
    shuffle(array, array+size, random_device());
}

void warmUp(const int* array, size_t size) {
    for (volatile size_t k = 0, i = 0; i < size; ++i) k = array[k];
}

uint64_t getTick() {
    unsigned int low, high;
    __asm__ __volatile__("rdtsc" : "=a"(low), "=d"(high));
    return (static_cast<unsigned long long>(high) << 32) | low;
}

uint64_t traverseTicks(const int* array, size_t size) {
    auto start = getTick();
    for (volatile size_t k = 0, i = 0; i < size; ++i) {
        k = array[k];
    }
    auto end = getTick();
    return (end - start)/size;
}

uint64_t minTraverseTime(const int* array, size_t size) {
    uint64_t min = UINT64_MAX;
    for (size_t i = 0; i < 10; ++i) {
        auto ticks = traverseTicks(array, 10*size);
        min = min > ticks ? ticks : min;
    }
    return min;
}
