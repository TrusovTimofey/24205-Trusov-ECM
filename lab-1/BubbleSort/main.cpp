#include <iostream>
#include <time.h>
#include <cstdlib>

void bubbleSort(int* arr, size_t len ){
    for (size_t i = 0; i < len - 1 ; ++i) {
        for (int j = 0; j < len - i - 1; ++j) {
            if(arr[j]>arr[j+1]) std::swap(arr[j],arr[j+1]);
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Не один аргумент" << std::endl;
        return 1;
    }
    
    int number = std::atoi(argv[1]);
    
    if (number <= 0) {
        std::cout << "Аргумент должен быть натуральным числом" << std::endl;
        return 1;
    }

    int* array = (int*)malloc(number * sizeof(int));
    if(array == nullptr){
        std::cout << "Не удалось выделить память" << std::endl;
        return 1;
    }
    
    for (int i=0; i<number; ++i)
    {
        array[i] = rand();
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    bubbleSort(array,(size_t)number);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);  
    
    double delta = (end.tv_sec - start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));
    std::cout << delta << "sec  " << (1 / (delta*1000000000)) << "%"<< std::endl;
    
    free(array);
    
    return 0;
}
