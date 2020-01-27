#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int i, arraySize, *a;

struct Part {
	int low;
	int high;
};

/* Prototypes */
void swap(int[], int, int);
void *quicksort(void *struc);
int partitioning(int low, int high);

/* Declarations */

void display(int a[], int length)
{
	int i;
	printf(">");
	for (i = 0; i < length; i++)
		printf(" %d", a[i]);
	printf("\n");
}

int partitioning(int low, int high){

    int pivot = a[high];
    int wall = low - 1;
    int current;

    for(current = low; current <= high - 1; current++){
        if(a[current] < pivot){
            wall += 1;
            swap(a, wall, current);
        }
    }
    if(a[high] < a[wall + 1]){
        swap(a, wall + 1, high);
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

void swap(int a[], int left, int right)
{
	int temp;
	temp = a[left];
	a[left] = a[right];
	a[right] = temp;
}

int main(int argc, char *argv[])
{
	int length = 0;
	FILE *fh;
	int data;

	if (argc != 2) {
		printf("usage: %s <filename>\n", argv[0]);
		return 1;
	}

	/* Initialize data. */
	printf("attempting to sort file: %s\n", argv[1]);

	fh = fopen(argv[1], "r");
	if (fh == NULL) {
		printf("error opening file\n");
		return 0;
	}

	while (fscanf(fh, "%d", &data) != EOF) {
		++arraySize;
		a = (int *) realloc(a, arraySize * sizeof(int));
		a[arraySize - 1] = data;
		display(a, arraySize);
	}
	fclose(fh);
	printf("%d elements read\n", arraySize);

    struct Part partition = {0, arraySize - 1};

	//display(a, length);

	pthread_t start;
	pthread_create(&start, NULL, quicksort, &partition);
	pthread_join(start, NULL);

	display(a, arraySize);

	pthread_exit(NULL);

	return 0;
}