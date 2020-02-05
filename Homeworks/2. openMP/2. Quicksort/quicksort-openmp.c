/* Quicksort algorithm utilizing multiple POSIX threads

    Features: works with both input file as well as generating an array with randomized values 

    usage under Linux:
            gcc quicksort.c -o quicksort -lpthread
            ./quicksort <input file>
                or
            ./quicksort arraySize range
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX_SIZE 1000
#define MAX_RANGE 10000

int arraySize, *array;
struct Part {
	int low;
	int high;
};

void print(int [], int);
void swap(int[], int, int);
void *quicksort(void *struc);
int partitioning(int low, int high);

void print(int array[], int length) {
	int i;
	printf("> [");
	for (i = 0; i < length; i++)
		printf(" %d", array[i]);
	printf(" ]\n");
}

void swap(int array[], int left, int right) {
	int temp;
	temp = array[left];
	array[left] = array[right];
	array[right] = temp;
}

int partitioning(int low, int high) {

    int pivot = array[high];
    int wall = low - 1;
    int current;

    for(current = low; current <= high - 1; current++){
        if(array[current] < pivot){
            wall += 1;
            swap(array, wall, current);
        }
    }
    if(array[high] < array[wall + 1]){
        swap(array, wall + 1, high);
    }
    return wall + 1;
}

void* quicksort(void* struc){
    struct Part * range = (struct Part *) struc;
    int low = range->low;
    int high = range->high;

    if(low < high){
        if(low < 0)
            low = 0;
        int p = partitioning(low, high);
        struct Part left = {low, p - 1};
        struct Part right = {p + 1, high};

        #pragma omp task 
        {
        quicksort(&left);
        }
        //printf("nr of threads = %d\n", omp_get_num_threads());
        #pragma omp task
        {
        quicksort(&right);
        }  
    }
    return 0;
}

int main(int argc, char *argv[]) {

	FILE *fh;
	int data, range;
    double start_time, end_time;

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

    struct Part partition = {0, arraySize - 1};


    int numThreads = (argc > 3) ? atoi(argv[3]) :100;
    omp_set_num_threads(numThreads);
    
    printf("Sorting array...\n");

    start_time = omp_get_wtime();
    quicksort(&partition);
    end_time = omp_get_wtime();
    
    printf("Sorted in %f ms \nSorted array:\n", (end_time - start_time)*1000);
	print(array, arraySize);

	return 0;
}