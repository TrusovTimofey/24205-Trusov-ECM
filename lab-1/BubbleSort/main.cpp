#include <iostream>
#include <time.h>
#define ARRAY_LENGTH 80000

void bubbleSort(int* arr, size_t len ){
    for (size_t i = 0; i < len - 1 ; ++i) {
        for (int j = 0; j < len - i - 1; ++j) {
            if(arr[j]>arr[j+1]) std::swap(arr[j],arr[j+1]);
        }
    }
}

int main() {
    int array[ARRAY_LENGTH] = {0};
    for (int & i : array)
    {
        i = rand();
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    bubbleSort(array,ARRAY_LENGTH);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);  
    std::cout << (end.tv_sec - start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec)) << std::endl;
    
    return 0;
}
