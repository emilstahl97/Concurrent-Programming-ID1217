/**
 * Unisex toilet simulation with males and females represented as
 * threads.
 *
 * Compile by using make file rule unisex.out. (Compiling is
 * also done on demand when running the program.)
 *
 * Run the program through default make rule. (run)
 * To specify number of males set NRM and to specify number of 
 * females set NRW.
 *
 * Example usage: 
 * > NRM=10 NRW=10 make
 */

#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define SHARED 0        /* The semaphores should not be shared. */
#define MAX_MEN 10     /* The max number of males possible. */
#define MAX_WOMEN 10     /* The max number of women possible. */
#define MAXTIMES 20     /* Max number of times the bathroom is used. */
#define MAXTIMEIN 3     /* Max seconds in the bathroom. */
#define MINTIMEIN 1     /* Min seconds in the bathroom. */
#define MAXINTERVAL 5   /* Max time between visits to the bathroom. */
#define MININTERVAL 1   /* Min time between visits to the bathroom. */

#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"


void * male(void *);    /* Male method declaration. */
void * female(void *);  /* Female method declaration. */

sem_t crit_sem, waiting_male_sem, waiting_female_sem; /* Semaphores used. */

/* Counters used. */
int males_inside = 0; 
int females_inside = 0; 
int males_waiting = 0; 
int females_waiting = 0;
int times_used;
 
/* Bool to keep record of we should let the other of the same gender in.
 * (Only one thread should do this.) */
bool letting_in_people = false;
bool men_shrinking = false;
bool women_shrinking = false;

/* Number of males and females specified by the user. */
int numMen, numWomen;

/**
 * Main method, initiates all the semaphores and counters and then starts the
 * specified number of males and females. Then joines them together again after
 * the bathroom has been visited MAXTIMES number of times.
 */
int main(int argc, char ** argv)
{
    int i; /* Used to iterate over the threads. */
    pthread_t men, women;
    
    numWomen = (argc > 1) ? atoi(argv[1]) : MAX_WOMEN;
    numMen = (argc > 2) ? atoi(argv[2]) : MAX_MEN;
       
    /* Initialize the semaphores used. */
    sem_init(&crit_sem, SHARED, 1);
    sem_init(&waiting_male_sem, SHARED, 0);
    sem_init(&waiting_female_sem, SHARED, 0);

    srand(time(NULL)); /* Seed the randomizer to provide different results. */

    for(i = 0; i < numMen; i++) /* Creates the males. */
    {
        pthread_create(&men, NULL, male, NULL);
    }
    for(i = 0; i < numWomen; i++) /* Creates the females. */
    {
        pthread_create(&women, NULL, female, NULL);
    }
    for(i = 0; i < numWomen; i++) /* Joins the threads again. */
    {
        pthread_join(women, NULL);
    }

    for(i = 0; i < numWomen; i++) /* Joins the threads again. */
    {
        pthread_join(men, NULL);
    }

    return 0;
}

/**
 * Prints the current status of the bathroom and its two queues.
 */
void print_status()
{
    printf("\nINSIDE: M=%d, F=%d.\nWAITING: M=%d, F=%d.\n\n", males_inside, females_inside, males_waiting, females_waiting);
}

void *male(void *arg)
{
	int id = (int)arg;
	int i;
	for (i = 0; i < MAXTIMES; i++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % 4);

		/* Should I wait for bathroom + other stuff */
		sem_wait(&crit_sem);
		if (females_inside > 0 || men_shrinking)
		{
			males_waiting++;
			sem_post(&crit_sem);
			sem_wait(&waiting_male_sem);
		}
		males_inside++;
		if (males_waiting > 0)
		{
			males_waiting--;
			sem_post(&waiting_male_sem);
		}
		else
		{
			sem_post(&crit_sem);
		}

		/* Go to bathroom */
		printf("🚹  %d enters the bathroom. Visit: " CYN "%d\n" RESET, id, i + 1);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(&crit_sem);
		males_inside--;
		men_shrinking = true;

		/* Make sure no more men enter while men leave */
		if (males_inside == 0)
			men_shrinking = false;

		/* Prioritize women */
		if (males_inside == 0 && females_waiting > 0)
		{
			females_waiting--;
			sem_post(&waiting_female_sem);
		}
		else
		{
			sem_post(&crit_sem);
		}
	}
	pthread_exit(NULL);
}

void *female(void *arg)
{
	int id = (int)arg;
	int i;
	for (i = 0; i < MAXTIMES; i++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % 4);

		/* Should I wait for bathroom + other stuff */
		sem_wait(&crit_sem);
		if (males_inside > 0 || women_shrinking)
		{
			females_waiting++;
			sem_post(&crit_sem);
			sem_wait(&waiting_female_sem);
		}
		females_inside++;
		if (females_waiting > 0)
		{
			females_waiting--;
			sem_post(&waiting_female_sem);
		}
		else
		{
			sem_post(&crit_sem);
		}

		/* Go to bathroom */
		printf("🚺  %d enters the bathroom. Visit: " YEL "%d\n" RESET, id, i + 1);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(&crit_sem);
		females_inside--;

		/* Make sure no more women enter while women leave */
		women_shrinking = true;
		if (females_inside == 0)
			women_shrinking = false;

		/* Prioritize men */
		if (females_inside == 0 && males_waiting > 0)
		{
			males_waiting--;
			sem_post(&waiting_male_sem);
		}
		else
		{
			sem_post(&crit_sem);
		}
	}
	pthread_exit(NULL);
}
