/*Hungry birds problem: one parent_bird, multiple consumers

Description:
n baby birds, one parent bird. the baby birds eat out of
a common dish containing W numWorms. when the dish is empty,
the parent bird is signaled and refills it. only semaphores
are used for mutual exclusion and signaling.

Usage in Linux:

	gcc hungrybirds.c -lpthread
	./a.out n
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


#define MAX_BIRDS 10
#define MAX_WORMS 50
#define SHARED 1

//int numWorms;			// current number of numWorms in dish
int consumed[MAX_BIRDS];	// how many numWorms each bird has consumed
int food;

sem_t parent;
sem_t baby;
sem_t chirp;

/* refills dish when it's empty */
void *parent_bird(){

 while(1){
	/* waits to be signaled */
	sem_wait(&chirp);
    printf("Parent bird on hunt for more worms\n");
	food = rand()%10+1;
    printf("%d worms found\n", food);
	sem_post(&parent);
	sleep(1);
 }
}


/* eats worm. signals parent when dish is empty */
void *baby_bird(void *arg){

 long id = (long) arg;

 while(1){
	sem_wait(&baby);				// lock to access dish	
  		if(food == 0){
		/* dish is empty : signal parent */
		printf("baby bird %ld. waking parent bird\n", id);
		sem_post(&chirp);
		sem_wait(&parent);

	}  
	food--;
	printf("Bird %d eats worm %d\n", id, food);
	sem_post(&baby);
	sleep(1);
 }

}

int main(int argc, char *argv[]) {
    
    int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    food = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;

    printf("in main worms =%d\n",food);
    
 pthread_t birds[numBirds];
 pthread_t parent;

  sem_init(&parent, SHARED, 0);  /* sem empty = 1 */
  sem_init(&baby, SHARED, 1);   /* sem full = 0  */
  sem_init(&chirp, SHARED, 0);   /* sem full = 0  */

/* creating threads to represent birds */
 pthread_create(&parent, NULL, parent_bird, NULL);

 for(id = 0; id < numBirds; id++)
	pthread_create(&birds[id], NULL, baby_bird, (void*)id);

/* wait for threads to terminate (which they, incidentally, won't) */
 pthread_join(parent, NULL);
 for(id = 0; id < numBirds; id++)
 	pthread_join(birds[id], NULL);

 return 0;

}