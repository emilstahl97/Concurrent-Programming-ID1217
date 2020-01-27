#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAXSIZE 1000

int i, arraySize, *array;

struct Part {
	int low;
	int high;
};

/* Prototypes */
void swap(int[], int, int);
void *quicksort(void *struc);
int partitioning(int low, int high);

/* Declarations */

void print(int array[], int length)
{
	int i;
	printf("> [");
	for (i = 0; i < length; i++)
		printf(" %d", array[i]);
	printf(" ]\n");
}

int partitioning(int low, int high){

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
    pthread_t th1, th2;

    if(low < high){
        if(low < 0)
            low = 0;
        int p = partitioning(low, high);
        struct Part left = {low, p - 1};
        pthread_create(&th1, NULL, quicksort, &left);

        struct Part right = {p + 1, high};
        pthread_create(&th2, NULL, quicksort, &right);
    }
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_exit(NULL);
}

void swap(int array[], int left, int right)
{
	int temp;
	temp = array[left];
	array[left] = array[right];
	array[right] = temp;
}

int main(int argc, char *argv[])
{
	FILE *fh;
    clock_t c_start, c_stop;
    double exTime = 0;
	int length = 0;
	int data, range;

	/* Initialize data. */
	fh = fopen(argv[1], "r");

	if (fh == NULL) {
        arraySize = (argc > 1)? atoi(argv[1]) : MAXSIZE;
        range = (argc > 2)? atoi(argv[2]) : 1000;
		printf("\nInitializing array with %d elements between 0 and %d\n", arraySize, range);
        array = malloc(sizeof(int) * arraySize);
        
        //Initialize matrix with arraySize and range
        srand(time(NULL));
        for(i = 0; i < arraySize; i++)
            array[i] = rand() % range;   
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

    printf("Sorting array...\n");
	pthread_t start;
    c_start =  clock();
	pthread_create(&start, NULL, quicksort, &partition);
	pthread_join(start, NULL);
    c_stop = clock();
    exTime = ((double)(c_stop - c_start)) / ((double)CLOCKS_PER_SEC/1000);

    printf("Sorted in %f ms \n", exTime);
    printf("Sorted array:\n");
	print(array, arraySize);

	pthread_exit(NULL);

	return 0;
}