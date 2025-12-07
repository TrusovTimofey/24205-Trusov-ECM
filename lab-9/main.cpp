#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

int* createArrayWithOffset(size_t elements, size_t fragments, size_t offset);

void warmUp(const int* array, size_t size);

uint64_t getTick();
uint64_t traverseTicks(const int* array, size_t size);
uint64_t minTraverseTime(const int* array, size_t size);

int main() {
    std::ofstream out("out.csv");
    out << "fragments,ticks/element\n"; 

    size_t offset = 1;
    offset <<= 25;

    size_t L1d = 48 * 1024; // 48КБ
    size_t elements = L1d / sizeof(int) / 2;

    size_t maxFragments = 32;
    for (size_t f = 1; f <= maxFragments; ++f) {
        
        auto array = createArrayWithOffset(elements,f,offset);
        //warmUp(array,elements);
        auto time = minTraverseTime(array, elements);
        out << f << "," << time << "\n";
        
        delete array;

        cout << f*100./maxFragments << "%" << endl;
    }

    return 0;
}

int* createArrayWithOffset(size_t elements, size_t fragments, size_t offset){
    elements/=fragments;
    int* arr = new int[offset*fragments];

    fragments--;

    for (size_t f = 0; f < fragments; f++)
    {
        for (size_t i = 0; i < elements; i++)
        {
            arr[offset*f + i] = offset*f + i + offset;
        }
        
    }

    for (size_t i = 0; i < elements; i++)
    {
        arr[offset*fragments + i-1] = i;
    }

    return arr;
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
        auto ticks = traverseTicks(array, 50*size);
        min = min > ticks ? ticks : min;
    }
    return min;
}
