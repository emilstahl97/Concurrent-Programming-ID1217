/*  This program simulates a consuumer/producer with bees and bears. The program initiates a number of honeybees and a bear.
	Given are n honeybees and a hungry bear. They share a pot of honey. The pot is initially empty; its capacity is H portions of honey. The bear sleeps until the pot is full, 
	then eats all the honey and goes back to sleep. Each bee repeatedly gathers one portion of honey and puts it in the pot; the bee who fills the pot awakens the bear.

   usage under Linux:
            gcc honeyBees.c -o honeyBees -lpthread
            ./honeyBees numBees
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define HONEY 150
#define MAX_BEES 25
#define MAX_HONEY 20
#define EATEN 0
#define SHARED 1

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
        printf("honeypot before eating = %d\n", honeyPot);
		printf("---------------The bear is eating all the honey from the honeyPot---------------\n");
		honeyPot = EATEN;
		sleep(2);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]) {

	int numBees, id;
	pthread_t bearT, bees[numBees];
	
	numBees = (argc > 1) ? atoi(argv[1]) : MAX_BEES;

	sem_init(&wakeUp, SHARED, 0);
	sem_init(&empty, SHARED, 1);

	for(id = 0; id < numBees; id++)
		pthread_create(&bees[id], NULL, bee, (void*)id);
	pthread_create(&bearT, NULL, bear, NULL);
	
	for(id = 0; id < numBees; id++)
		pthread_join(bees[id], NULL);
	pthread_join(bearT, NULL);
	
	return 0;
}