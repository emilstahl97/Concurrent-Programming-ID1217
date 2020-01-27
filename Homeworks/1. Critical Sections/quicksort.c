#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void display(int array[], int length)
{
	int i;
	printf(">");
	for (i = 0; i < length; i++)
		printf(" %d", array[i]);
	printf("\n");
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
    clock_t c_start, c_stop;
    double exTime = 0;
	int length = 0;
	int data;
    int arraySize = atoi(argv[1]);

    if(arraySize == 0) {
            printf("input is a file\n");

	FILE *fh;

	if (argc != 2) {
		printf("usage: %s <filename>\n", argv[0]);
		return 1;
	}

	printf("attempting to sort file: %s\n", argv[1]);

	fh = fopen(argv[1], "r");
	if (fh == NULL) {
		printf("error opening file\n");
		return 0;
	}
    printf("Reading the file:\n");
	while (fscanf(fh, "%d", &data) != EOF) {
		++arraySize;
		array = (int *) realloc(array, arraySize * sizeof(int));
		array[arraySize - 1] = data;
		display(array, arraySize);
	}
	fclose(fh);
	printf("%d elements read\n", arraySize);
    }
    else {
        printf("input is a digit");
         struct Part partition = {0, arraySize - 1};

    array = malloc(sizeof(int) * arraySize);

    srand(time(NULL));
    for(i = 0; i < arraySize; i++){
        array[i] = rand() % 1000;
        //printf("%d ", a[i]);
    }
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
	display(array, arraySize);

	pthread_exit(NULL);

	return 0;

}