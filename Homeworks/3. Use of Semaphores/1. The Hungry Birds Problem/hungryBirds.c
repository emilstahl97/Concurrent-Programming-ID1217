/*  This program simulates a consuumer/producer with birds. The program initiates a number of baby birds and a parent bird.
	The baby birds eat out of a common dish that initially contains W worms. Each baby bird repeatedly takes a worm, eats it, sleeps for a while, takes another worm, and so on. 
	If the dish is empty, the baby bird who discovers the empty dish chirps real loud to awaken the parent bird. The parent bird flies off and gathers W more worms, puts them in the dish, and then waits for the dish to be empty again. 
	This pattern repeats forever.
    
   usage under Linux:
            gcc hungryBirds.c -o hungryBird -lpthread
            ./hungryBirds numBirds numWorms
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SLEEP 10
#define MAX_BIRDS 10
#define MAX_WORMS 50
#define MAX_REFILL numBirds*2
#define SHARED 1
//#define DEBUG

sem_t empty, full;
int worms, refill, sleepInterval;

void *baby_bird(void *arg) {
	int id = (int) arg;
	int numEaten = 0;
	while(1) {
		sem_wait(&full);
		if (worms > 0) {
			worms--;
			numEaten++;
			printf("Bird %d ate worm %d\n", id, worms);
			#ifdef DEBUG
			printf("Bird %d has eaten %d worms\n", id, numEaten);
			#endif
			sem_post(&full);
			sleep(2);
			printf("Bird %d sleeps\n",id);
		} else {
			printf("Bird %d SQUEELS!!!!!!!!!!!!!!!!!!!!!\n", id);
			sleep(2);
			sem_post(&empty);
		}
	}
}

void *parent_bird() {
	while(1) {
		sem_wait(&empty);
		if(worms == 0);
		worms = rand()%refill;
		printf("**********Parent bird awake**********************\n");
		printf("**********Parent bird found %d new worms*********\n", worms);
		printf("**********Parent bird sleeping*******************\n");
		sleep(1);
		sem_post(&full);
	}
}

int main(int argc, char *argv[]) {
	
	int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    worms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
	refill = (argc > 3) ? atoi(argv[3]) : MAX_REFILL;
	sleepInterval = (argc > 4) ? atoi(argv[4]) : MAX_SLEEP;
	
	pthread_t birds[numBirds];
	pthread_t parent;
	
	/* initiate semaphores */
	sem_init(&empty, SHARED, 0); 			// worms starts out as full
	sem_init(&full, SHARED, 1);				// mutex initialized to 1 to indicate critical section is free  	
	
	/* creating threads to represent birds */
	for(id = 0; id < numBirds; id++)
		pthread_create(&birds[id], NULL, baby_bird, (void*)id);
	pthread_create(&parent, NULL, parent_bird, NULL);
	
	/* wait for threads to terminate (which they, incidentally, won't) */
	for(id = 0; id < numBirds; id++)
		pthread_join(birds[id], NULL);
	pthread_join(parent, NULL);
	
	return 0;

}