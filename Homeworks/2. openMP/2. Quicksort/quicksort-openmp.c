#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define MAX_THREADS 36
#define MAX_SIZE 1000
#define MAX_RANGE 10000
#define LOW_LIMIT 1000

void print(int [], int);
static void insertionSort(int [], int);
static int partition(int, int, int*);
static void quicksort(int, int, int*);

void print(int array[], int length) {
	int i;
	printf("> [");
	for (i = 0; i < length; i++)
		printf(" %d", array[i]);
	printf(" ]\n");
}

/* When list is < 1000 elements, insertionSort is used */
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

	if ((high - pivot) < LOW_LIMIT) {
		return insertionSort(&list[pivot], high - pivot);
    }

	int mid = partition(pivot, high, list);

    #pragma omp task
    {
	    quicksort(pivot, mid - 1, list);
    }
    #pragma omp task
    {
	    quicksort(mid + 1, high, list);
    }
}

int main(int argc, char *argv[])
{
	FILE *fh;
    double exTime = 0;
	int data, range, length, numThreads, *array;

	/* Initialize data. */
	fh = fopen(argv[1], "r");

	if (fh == NULL) {
        length = (argc > 1)? atoi(argv[1]) : MAX_SIZE;
        range = (argc > 2)? atoi(argv[2]) : MAX_RANGE;
		//printf("\nInitializing array with %d elements between 0 and %d\n", length, range);
        array = malloc(sizeof(int) * length);
        
        //Initialize matrix with arraySize and range
        srand(time(NULL));
        for(int i = 0; i < length; i++)
            array[i] = rand() % range; 
        #ifdef PRINT  
        print(array, length);
        #endif
	}
    else {
        printf("Reading the file:\n");
	    while (fscanf(fh, "%d", &data) != EOF) {
		    length++;
		    array = (int *) realloc(array, length * sizeof(int));
		    array[length - 1] = data;
		    print(array, length);
	    }
	    fclose(fh);
	    printf("%d elements read\n", length);
    }

    numThreads = (argc > 3) ? atoi(argv[3]) : MAX_THREADS;
	omp_set_num_threads(numThreads);

    double start_time = omp_get_wtime();
    #pragma omp parallel
	{
    #pragma omp single nowait
		quicksort(0, length - 1, &array[0]);
	}
	double end_time = omp_get_wtime();
    printf("Sorted in %g ms:\n", (end_time-start_time)*1000);
    #ifdef PRINT
	printf("Sorted array\n");
    print(array, length);
    #endif

	/* Free  array and return */
	free(array);
	return 0;
}