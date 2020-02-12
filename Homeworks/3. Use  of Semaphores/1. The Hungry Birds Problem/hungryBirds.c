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

sem_t baby, parent, chirp;

/* refills dish when it's empty */
void *parent_bird(){

 while(1){
	/* waits to be signaled */
	sem_wait(&chirp);
	printf("Parent on hunt for more worms\n");
	numWorms = rand()%10+1;
    printf("%d worms found\n", numWorms);
	sem_post(&parent);
	sleep(1);
 }
}


/* eats worm. signals parent when dish is empty */
void *baby_bird(void *arg){

 int id = (long) arg;

 while(1){
	 
	 sem_wait(&baby);
	 if(numWorms == 0) {
		 printf("Bird %d chirps for food!\n", id);
		 sem_post(&chirp);
		 sem_wait(&parent);
	 }
	 numWorms--;
	 printf("Bird %d eats worm %d \n", id, numWorms);
	 sem_post(&baby);
	 sleep(1);

 }
 
 }

int main(int argc, char *argv[]) {
    
    int numBirds, id;
    
    numBirds = (argc > 1) ? atoi(argv[1]) : MAX_BIRDS;
    numWorms = (argc > 2) ? atoi(argv[2]) : MAX_WORMS;
    
 pthread_t birds[numBirds];
 pthread_t parent;

/* initiate semaphores */
sem_init(&parent, SHARED, 0);  /* sem empty = 1 */
  sem_init(&baby, SHARED, 1);   /* sem full = 0  */
  sem_init(&chirp, SHARED, 0);   /* sem full = 0  */


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