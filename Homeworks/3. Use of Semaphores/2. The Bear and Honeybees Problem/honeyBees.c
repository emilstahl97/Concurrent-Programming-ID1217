#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define HONEY 150
#define MAX_BEES 25
#define MAX_HONEY 20
#define EATEN 0

sem_t wakeUp, empty;
int honeyPot = 0;


void *bee(void *arg) {
	int id = (int) arg;
	while(1) {
		sem_wait(&empty);
		honeyPot++;
		printf("Bee %d is adding one portion of honey into the honeyPot(%d)\n", id, honeyPot);
		if (honeyPot == MAX_HONEY) {
			printf("---------------Bee %d is waking the bear up---------------\n", id);
			sem_post(&wakeUp);
			sleep(1);
		} else {
			sem_post(&empty);
			sleep(1);
		}
	}
}

void *bear() {
	while(1) {
		sem_wait(&wakeUp);
		printf("---------------The bear is eating all the honey from the honeyPot---------------\n");
		honeyPot = EATEN;
		sleep(1);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]) {

	int numBees, id;
	numBees = (argc > 1) ? atoi(argv[1]) : MAX_BEES;

	sem_init(&wakeUp, 1, 0);
	sem_init(&empty, 1, 1);

	pthread_t bearT, bees[numBees];
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