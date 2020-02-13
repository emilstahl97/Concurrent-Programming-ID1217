/*  This program simulates a consuumer/producer with bees. The program initiates a number of baby bees and a bear bird.
	The baby bees eat out of a common dish that initially contains W worms. Each baby bird repeatedly takes a worm, eats it, sleeps for a while, takes another worm, and so on. 
	If the dish is wakeUp, the baby bird who discovers the wakeUp dish chirps real loud to awaken the bear bird. The bear bird flies off and gathers W more worms, puts them in the dish, and then waits for the dish to be wakeUp again. 
	This pattern repeats forever.
    
   usage under Linux:
            gcc hungryBirds.c -o hungryBird -lpthread
            ./hungryBirds numBees numWorms
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SLEEP 10
#define MAX_BEES 10
#define FULL 50
#define EATEN 0
#define SHARED 1

sem_t wakeUp, empty;
int honeyPot = 0;

void *bee(void *arg) {
	int id = (int) arg;
	while(1) {
		sem_wait(&empty);
        honeyPot++;
        printf("Bee nr %d added some honey to the pot = %d\n", id, honeyPot);
        if(honeyPot == FULL) {
            printf("Honeypot is full! Bee nr %d waking up the bear\n", id);
            sem_post(&wakeUp);
            sleep(1);
        }
        else {
            sem_post(&empty);
            sleep(1);
        }
	}
}

void *bear() {
	while(1) {
		sem_wait(&wakeUp);
        printf("Bear awake!\n");
        printf("The bear ate all the honeey and is now going back to sleep\n");
		honeyPot = EATEN;
        sleep(1);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]) {
	
	int numBees, id;
    
    numBees = (argc > 1) ? atoi(argv[1]) : MAX_BEES;
	
	pthread_t bearT, bees[numBees];
	
	/* initiate semaphores */
	sem_init(&wakeUp, SHARED, 0); 			// worms starts out as empty
	sem_init(&empty, SHARED, 1);				// mutex initialized to 1 to indicate critical section is free  	
	
	/* creating threads to represent bees */
	for(id = 0; id < numBees; id++)
		pthread_create(&bees[id], NULL, bee, (void*)id);
	pthread_create(&bearT, NULL, bear, NULL);
	
	/* wait for threads to terminate (which they, incidentally, won't) */
	for(id = 0; id < numBees; id++)
		pthread_join(bees[id], NULL);
	pthread_join(bearT, NULL);
	
	return 0;

}