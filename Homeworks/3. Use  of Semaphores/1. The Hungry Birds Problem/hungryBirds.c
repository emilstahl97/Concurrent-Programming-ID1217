
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BIRDS 10
#define MAX_WORMS 50
#define MAX_REFILL numBirds*2
#define SHARED 1

sem_t empty, full;
int worms, refill;

void *parent_bird() {
	while(1) {
		sem_wait(&empty);
		if(worms == 0);
		worms = rand()%refill;
		printf("---------------Parent: Refilled with %d new worms---------------\n", worms);
		sleep(1);
		sem_post(&full);
	}
}

void *baby_bird(void *arg) {
	int id = (int) arg;
	while(1) {
		sem_wait(&full);
		if (worms > 0) {
			worms--;
			printf("Bird %d ate worm %d\n", id, worms);
			sem_post(&full);
			sleep(1);
		} else {
			printf("Bird %d SQUEELS!!!!!!!!!!!!!!!!!!!!!\n", id);
			sleep(rand()%3);
			sem_post(&empty);
		}
	}
}


int main(int argc, char *argv[]) {
	
	int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    worms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
	refill = (argc > 3) ? atoi(argv[2]) : MAX_REFILL;
	
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