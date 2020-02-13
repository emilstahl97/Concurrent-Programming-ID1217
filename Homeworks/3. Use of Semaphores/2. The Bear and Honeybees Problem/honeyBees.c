#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define HONEY 150
#define HONEYBEES 25

sem_t wakeUp, empty;
int pot = 0;
int honey;


void *bee(void *arg) {
	int id = (int) arg;
	while(1) {
		sem_wait(&empty);
		pot++;
		printf("Bee %d is adding one portion of honey into the pot(%d)\n", id, pot);
		if (pot == honey) {
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
		printf("---------------The bear is eating all the honey from the pot---------------\n");
		pot = 0;
		sleep(1);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 3 || argc < 3) {
		printf("ERROR: Input arguments need to be exactly 2\n");
		exit(-1);
	}
	int numHoneyBees;
	numHoneyBees = (argc > 1) ? atoi(argv[1]) : HONEYBEES;
	honey = (argc > 2) ? atoi(argv[2]) : honey;
	if (numHoneyBees > HONEYBEES) numHoneyBees = HONEYBEES;
	if (honey > HONEY) honey = HONEY;
	printf("The number of honeybees are %d and the pot can contain %d portions of honey\n", numHoneyBees, honey);
	printf("********* STARTING *********\n");
	sleep(2);

	sem_init(&wakeUp, 1, 0);
	sem_init(&empty, 1, 1);

	pthread_t bees[HONEYBEES];
	pthread_t bearT;
	int i;
	pthread_create(&bearT, NULL, bear, NULL);
	for (i = 0; i < numHoneyBees; i++) {
		pthread_create(&bees[i], NULL, bee, (void*) i);
	}
	pthread_join(bearT, NULL);
	for (i = 0; i < numHoneyBees; i++) {
		pthread_join(bees[i], NULL);
	}
	
	return 0;
}