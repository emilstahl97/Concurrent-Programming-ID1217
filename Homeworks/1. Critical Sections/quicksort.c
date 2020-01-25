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
        /*struct Part *left = malloc(sizeof(struct Part));
        left->low = low;
        left->high = p - 1;*/
        struct Part left = {low, p - 1};
        pthread_create(&th1, NULL, quicksort, &left);

        /*struct Part *right = malloc(sizeof(struct Part));
        right->low = p + 1;
        right->high = high;*/
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
	arraySize = (argc > 1) ? atoi(argv[1]) : 15;
	
	struct Part partition = {0, arraySize - 1};

	array = malloc(sizeof(int) * arraySize);

	srand(time(NULL));
	for(i = 0; i < arraySize; i++) { 
		array[i] = rand() % 10; 
	}

	//display(array, length);

	pthread_t start;
	pthread_create(&start, NULL, quicksort, &partition);
	pthread_join(start, NULL);

	display(array, arraySize);

	pthread_exit(NULL);

	return 0;
}