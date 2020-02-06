#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define MAX_SIZE 1000
#define MAX_RANGE 10000
#define LOW_LIMIT 1000
int arraySize, *array;

void print(int [], int);

void print(int array[], int length) {
	int i;
	printf("> [");
	for (i = 0; i < length; i++)
		printf(" %d", array[i]);
	printf(" ]\n");
}


/* When list gets small */
static void insertionSort(int arr[], int n)
{
	int i, j;
	float temp;
	for (i = 1; i < n; i++)
	{
		temp = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > temp)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

/* Meat of quick sort. Partitioning */
static int partition(int pivot_index, int high, int *data)
{
	float pivot = data[pivot_index];
	int k = pivot_index + 1;
	int l = high;
	float temp;

	while (k < l)
	{
		temp = data[k];
		data[k] = data[l];
		data[l] = temp;
		while (data[k] <= pivot)
			k++;
		while (data[l] > pivot)
			l--;
	}
	data[pivot_index] = data[l];
	data[l] = pivot;
	return l;
}

/* The parallel quicksort */
static void quicksort(int pivot, int high, int *list)
{
	if (pivot >= high)
		return;

	if ((high - pivot) < LOW_LIMIT)
		return insertionSort(&list[pivot], high - pivot);

	int mid = partition(pivot, high, list);

#pragma omp task
	quicksort(pivot, mid - 1, list);
#pragma omp task
	quicksort(mid + 1, high, list);
}



int main(int argc, char *argv[]) {

	FILE *fh;
    double start_time, end_time;  // start and end times
	int data, range;

	/* Initialize data. */
	fh = fopen(argv[1], "r");

	if (fh == NULL) {
        arraySize = (argc > 1)? atoi(argv[1]) : MAX_SIZE;
        range = (argc > 2)? atoi(argv[2]) : MAX_RANGE;
		printf("\nInitializing array with %d elements between 0 and %d\n", arraySize, range);
        array = malloc(sizeof(int) * arraySize);
        
        //Initialize matrix with arraySize and range
        srand(time(NULL));
        for(int i = 0; i < arraySize; i++)
            array[i] = rand() % range;   
        print(array, arraySize);
	}
    else {
        printf("Reading the file:\n");
	    while (fscanf(fh, "%d", &data) != EOF) {
		    arraySize++;
		    array = (int *) realloc(array, arraySize * sizeof(int));
		    array[arraySize - 1] = data;
		    print(array, arraySize);
	    }
	    fclose(fh);
	    printf("%d elements read\n", arraySize);
    }

    printf("Sorting array...\n");

    start_time = omp_get_wtime();
    quicksort(0, arraySize-1, array);
    end_time = omp_get_wtime();

    printf("Sorted in %f ms \nSorted array:\n", (end_time - start_time)*1000);
	print(array, arraySize);

	return 0;
}