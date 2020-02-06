#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#define RUNONCE

/* Helper function */
double drand(double low, double high)
{
	return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}
int c = 0;
/* When list gets small */
static void insertionSort(float arr[], int n)
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
static int partition(int pivot_index, int high, float *data)
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
static void quicksort(int pivot, int high, float *list, int low_limit)
{
	if (pivot >= high)
		return;

	if ((high - pivot) < low_limit) {
        printf("Insertion sort enabled\n");
        #pragma omp critical
        {
        c++;
        }
		return insertionSort(&list[pivot], high - pivot);
    }

	int mid = partition(pivot, high, list);

#pragma omp task
	quicksort(pivot, mid - 1, list, low_limit);
#pragma omp task
	quicksort(mid + 1, high, list, low_limit);
    printf("doing quicksort\n");
}


/* Swap function for bubble sort */
void swap(double *xp, double *yp)
{
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

/* An optimized version of bubble sort */
void sort(double arr[], int n)
{
	int i, j;
	bool swapped;

	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
				swapped = true;
			}
		}

		// IF no two elements were swapped by inner loop, then break
		if (swapped == false)
			break;
	}
}


int main(int argc, char *argv[])
{
	int length =(argc > 1) ? atoi(argv[1]) : 100000;
	/* Init main array */
	float *mainArray;
	mainArray = (float *)malloc(sizeof(float) * length);
	if (mainArray == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	int i, j;
	for (i = 0; i < length; i++)
		mainArray[i] = drand(0.0, 100.0);
double start_time = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single nowait
		quicksort(0, length - 1, &mainArray[0], 1000);
	}
	double end_time = omp_get_wtime();
	/* Free main array and return */
	free(mainArray);
    printf("c = %d\n", c);
	return 0;
}