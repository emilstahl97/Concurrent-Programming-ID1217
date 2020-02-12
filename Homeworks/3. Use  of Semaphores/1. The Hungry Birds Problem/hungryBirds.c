#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BIRDS 10
#define MAX_WORMS 50

void* bird(void* arg);
void* parent(void* arg);

sem_t check_worm;
sem_t get_worm;
sem_t empty;

int worms;

int main(int argc, char* argv[]) {

    int numBirds, numWorms, i;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    numWorms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
    
    pthread_t birds[numBirds], parent;

    sem_init(&check_worm, 0, 0);
    sem_init(&get_worm, 0, 1);
    sem_init(&empty, 0, 0);


    for(i = 0; i < numBirds; i++) {
        pthread_create(&birds[i], NULL, bird, (void*)i);
    }

    pthread_create(&parent, NULL, parent, (void*)i);

    for(i = 0; i < numBirds; i++) {
        pthread_join(birds[i], NULL);
    }

    pthread_join(parent, NULL);


    return 0;

}

void *bird(void* arg) {

    int id = (int) arg;

    while(1) {

        sem_wait(&check_worm);
            if(worms == 0) {
                printf("bird %d waking parent\n", id);
                sem_post(&empty);
            } else {
                printf("bird %d is eating, %d worms left %d\n", id, worms);
                sem_post(&check_worm);
                sleep(rand()%10);
            }

    }
}

void* parent() {

    sem_wait(&empty);
    printf("refilling");
    worms = 10;
    sem_post(&check_worm);
}