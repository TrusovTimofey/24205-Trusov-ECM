#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

	int* array = (int*)malloc(number * sizeof(int));
	if(array == NULL) {
		printf("Не удалось выделить память\n");
		return 1;
	}

	for(int i = 0; i < number; ++i) {
		array[i] = rand();
	}

	struct timespec start, end, err;
    clock_getres(CLOCK_MONOTONIC_RAW, &err);
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	bubbleSort(array, (size_t)number);

	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    

	double delta = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);

    
	printf("%.10lfsec, %.15lf%%\n",delta,(err.tv_nsec / (delta * 1000000000)));

	free(array);

	return 0;
}
