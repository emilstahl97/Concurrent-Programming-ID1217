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

int numWorms;			// current number of numWorms in dish
int consumed[MAX_BIRDS];	// how many numWorms each bird has consumed

sem_t check_worm;
sem_t get_worm;
sem_t empty;

/* refills dish when it's empty */
void *parent_bird(){

 while(1){
	/* waits to be signaled */
	sem_wait(&empty);
	printf("parent bird. refilling dish\n");
	numWorms = rand()%10+1;
    printf("%d worms found\n", numWorms);
	sem_post(&check_worm);
 }
}


/* eats worm. signals parent when dish is empty */
void *baby_bird(void *arg){

 long myid = (long) arg;

 while(1){
	sem_wait(&check_worm);				// lock to access dish	
  		if(numWorms == 0){
		/* dish is empty : signal parent */
		printf("baby bird %ld. waking parent bird\n", myid);
		sem_post(&empty);
	} else {
		printf("baby bird %ld eating. %d numWorms left. %d numWorms consumed\n", myid, --numWorms, ++consumed[myid]);
		sem_post(&check_worm);			// unlock
		sleep(rand()%5);			// sleep for random amount of time
 	}
 }

}

int main(int argc, char *argv[]) {
    
    int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    numWorms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
    
 pthread_t birds[numBirds];
 pthread_t parent;

/* initiate semaphores */
 sem_init(&empty, SHARED, 0); 		// dish starts out as full
 sem_init(&check_worm, SHARED, 1);			// mutex initialized to 1 to indicate critical section is free  	

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