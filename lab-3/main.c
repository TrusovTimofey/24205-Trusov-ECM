#include <stdlib.h>
#include <stdio.h>

int* generateRandomArray(int size) {
    int* array = (int*)malloc(size * sizeof(int));
	for(int i = 0; i < size; ++i) {
		array[i] = rand();
	}
    return array;
}

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void bubbleSort(int* arr, size_t len) {
	for(size_t i = 0; i < len - 1; ++i) {
		for(int j = 0; j < len - i - 1; ++j) {
			if(arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);
		}
	}
}

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("Не один аргумент\n");
		return 1;
	}

	int number = atoi(argv[1]);

	if(number <= 0) {
		printf("Аргумент должен быть натуральным числом\n");
		return 1;
	}

	int* array = generateRandomArray(number);
    if(array == NULL) {
		printf("Не удалось выделить память\n");
		return 1;
	}

	bubbleSort(array, (size_t)number);

	free(array);

	return 0;
}
