/*Hungry birds problem: one producer, multiple consumers

Description:
n baby birds, one parent bird. the baby birds eat out of
a common dish containing W worms. when the dish is empty,
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
int n;	 		// number of baby birds
int worms;			// current number of worms in dish
int max = 10; 	// max number of worms in dish

int consumed[MAX_BIRDS];	// how many worms each bird has consumed

sem_t check_worm;
sem_t get_worm;
sem_t empty;

/* refills dish when it's empty */
void *producer(){

 while(1){
	/* waits to be signaled */
	sem_wait(&empty);
	printf("parent bird. refilling dish\n");
	worms = max;
	sem_post(&check_worm);
 }
}


/* eats worm. signals parent when dish is empty */
void *consumer(void *arg){

 long myid = (long) arg;

 while(1){
	sem_wait(&check_worm);				// lock to access dish	
  		if(worms == 0){
		/* dish is empty : signal parent */
		printf("baby bird %ld. waking parent bird\n", myid);
		sem_post(&empty);
	} else {
		printf("baby bird %ld eating. %d worms left. %d worms consumed\n", myid, --worms, ++consumed[myid]);
		sem_post(&check_worm);			// unlock
		sleep(rand()%5);			// sleep for random amount of time
 	}
 }

}

int main(int argc, char *argv[]) {
    
    int numBirds, numWorms, i;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    numWorms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
    

 worms = numWorms;
 pthread_t babyBirds[n];
 pthread_t parentBird;

/* initiate semaphores */
 sem_init(&empty, SHARED, 0); 		// dish starts out as full
 sem_init(&check_worm, SHARED, 1);			// mutex initialized to 1 to indicate critical section is free  	

/* creating threads to represent birds */
 pthread_create(&parentBird, NULL, producer, NULL);

 for(i = 0; i < n; i++)
	pthread_create(&babyBirds[i], NULL, consumer, (void*)i);

/* wait for threads to terminate (which they, incidentally, won't) */
 pthread_join(parentBird, NULL);
 for(i = 0; i < n; i++)
 	pthread_join(babyBirds[i], NULL);

 return 0;

}