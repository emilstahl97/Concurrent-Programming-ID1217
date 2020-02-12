
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BIRDS 10
#define MAX_WORMS 50
#define REFILL 10
#define SHARED 1

sem_t empty, full;
int worms;

void *parent_bird() {
	while(1) {
		sem_wait(&empty);
		if(worms == 0);
		worms = REFILL;
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
			printf("Bird %d ate from the worms(%d left)\n", id, worms);
			sem_post(&full);
			sleep(1);
		} else {
			printf("---------------Bird %d SQUEELS---------------\n", id);
			sleep(rand()%3);
			sem_post(&empty);
		}
	}
}


int main(int argc, char *argv[]) {



    int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    worms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
    
 pthread_t birds[numBirds];
 pthread_t parent;

/* initiate semaphores */
 sem_init(&empty, SHARED, 0); 		// worms starts out as full
 sem_init(&full, SHARED, 1);			// mutex initialized to 1 to indicate critical section is free  	

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