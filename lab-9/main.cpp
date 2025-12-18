#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

unsigned int* createArrayWithOffset(unsigned int elements, unsigned int fragments, unsigned int offset){
    elements/=fragments;
    unsigned int* arr = new unsigned int[offset*fragments];

    fragments--;

    for (unsigned int f = 0; f < fragments; f++)
    {
        for (unsigned int i = 0; i < elements; i++)
        {
            arr[offset*f + i] = offset*(f+1) + i;
        }
        
    }

    elements--;
    for (unsigned int i = 0; i < elements; i++)
    {
        arr[offset*fragments + i] = i+1;
    }

    return arr;
}

unsigned long long getTick() {
    unsigned int low, high;
    __asm__ __volatile__("rdtsc" : "=a"(low), "=d"(high));
    return (static_cast<unsigned long long>(high) << 32) | low;
}

unsigned long long traverseTicks(const unsigned int* array, unsigned int size) {
    auto start = getTick();
    for (volatile unsigned int k = 0, i = 0; i < size; ++i) {
        k = array[k];
    }
    auto end = getTick();
    return (end - start)/size;
}

unsigned long long minTraverseTime(const unsigned int* array, unsigned int size) {
    unsigned long long min = ~((unsigned long long)0);
    for (unsigned int i = 0; i < 100; ++i) {
        auto ticks = traverseTicks(array, 1000*size);
        min = min > ticks ? ticks : min;
    }
    return min;
}

int main() {
    std::ofstream out("out.csv");
    out << "fragments,ticks/element\n"; 

    const unsigned int offset = 45 << 21;
    const unsigned int L1d = 48 * 1024; // 48КБ
    const unsigned int elements = L1d / sizeof(unsigned int);
    const unsigned int maxFragments = 32;

    for (unsigned int fragments = 1; fragments <= maxFragments; ++fragments) {
        
        auto array = createArrayWithOffset(elements,fragments,offset);

        auto time = minTraverseTime(array, elements);
        out << fragments << "," << time << "\n";
        
        delete array;

        cout << (fragments * 100. / maxFragments) << "%" << endl;
    }

    return 0;
}
