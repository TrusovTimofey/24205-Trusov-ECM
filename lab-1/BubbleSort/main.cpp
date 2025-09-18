#include <iostream>
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

    bubbleSort(array,ARRAY_LENGTH);
        
    return 0;
}
