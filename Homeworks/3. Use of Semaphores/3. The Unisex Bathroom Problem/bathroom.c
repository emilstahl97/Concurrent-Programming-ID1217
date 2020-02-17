
#include "config.h"

sem_t lock, waiting_male, waiting_female; /* Semaphores used. */

/* Counters used. */
int males_inside = 0; 
int females_inside = 0; 
int males_queue = 0; 
int females_queue = 0;
 
/* Number of males and females specified by the user. */
int numMen, numWomen, numVisits;
char* man = 'm', woman = 'w';

/* Bool to keep record of we should let the other of the same gender in. */
bool men_leaving = false, women_leaving = false;

void * male(void *);    /* Male method declaration. */
void * female(void *);  /* Female method declaration. */
void print(char*, int, int);

/**
 * Main method, initiates all the semaphores and counters and then starts the
 * specified number of males and females. Then joines them together again after
 * the bathroom has been visited MAXTIMES number of times.
 */
int main(int argc, char* argv[])
{
    int i; /* Used to iterate over the threads. */
    pthread_t men, women;
    
    numWomen = (argc > 1) ? atoi(argv[1]) : MAX_WOMEN;
    numMen = (argc > 2) ? atoi(argv[2]) : MAX_MEN;
    numVisits = (argc > 3) ? atoi(argv[3]) : MAX_VISITS;
       
    /* Initialize the semaphores used. */
    sem_init(&lock, SHARED, 1);
    sem_init(&waiting_male, SHARED, 0);
    sem_init(&waiting_female, SHARED, 0);

    for(i = 0; i < numWomen; i++) /* Creates the females. */
    {
        pthread_create(&women, NULL, female, (void*)i);
    }

    for(i = 0; i < numMen; i++) /* Creates the males. */
    {
        pthread_create(&men, NULL, male, (void*)i);
    }

    for(i = 0; i < numWomen; i++) /* Joins the threads again. */
    {
        pthread_join(women, NULL);
    }

    for(i = 0; i < numMen; i++) /* Joins the threads again. */
    {
        pthread_join(men, NULL);
    }

    sem_close(&lock);
    sem_close(&waiting_female);
    sem_close(&waiting_male);

    return 0;
}

void print(char* gender, int id, int v)
{
    int visits = (int)v;
    if(gender == 'w')
	printf("🚺 %d enters the bathroom - Visit: " LMAG "#%d\n" RESET, id, visits + 1);
    if(gender == 'm') {
	printf("🚹 %d enters the bathroom - Visit: " LBLU "#%d\n" RESET, id, visits + 1);
    }
}

void *male(void *arg)
{
	int visit, id = (int)arg;

	for (visit = 0; visit < numVisits; visit++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % MAX_INTERVAL);

		/* Should I wait for bathroom + other stuff */
		sem_wait(&lock);
		if (females_inside > 0 || men_leaving)
		{
			males_queue++;
			sem_post(&lock);
			sem_wait(&waiting_male);
		}
		males_inside++;
		if (males_queue > 0)
		{
			males_queue--;
			sem_post(&waiting_male);
		}
		else
		{
			sem_post(&lock);
		}

		/* Go to bathroom */
		print(man, id, visit);
		sleep(rand() % MAX_TIMEIN);

		/* What to do after finished with bathroom visit */
		sem_wait(&lock);
		males_inside--;
		men_leaving = true;

		/* Make sure no more men enter while men leave */
		if (males_inside == 0)
			men_leaving = false;

		/* Prioritize women */
		if (males_inside == 0 && females_queue > 0)
		{
			females_queue--;
			sem_post(&waiting_female);
		}
		else
		{
			sem_post(&lock);
		}
	}
	pthread_exit(NULL);
}

void *female(void *arg)
{
	int id = (int)arg;
	int visit;
	for (visit = 0; visit < numVisits; visit++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % MAX_INTERVAL);

		/* Should I wait for bathroom + other stuff */
		sem_wait(&lock);
		if (males_inside > 0 || women_leaving)
		{
			females_queue++;
			sem_post(&lock);
			sem_wait(&waiting_female);
		}
		females_inside++;
		if (females_queue > 0)
		{
			females_queue--;
			sem_post(&waiting_female);
		}
		else
		{
			sem_post(&lock);
		}

		/* Go to bathroom */
        print(woman, id, visit);
		sleep(rand() % MAX_TIMEIN);

		/* What to do after finished with bathroom visit */
		sem_wait(&lock);
		females_inside--;

		/* Make sure no more women enter while women leave */
		women_leaving = true;
		if (females_inside == 0)
			women_leaving = false;

		/* Prioritize men */
		if (females_inside == 0 && males_queue > 0)
		{
			males_queue--;
			sem_post(&waiting_male);
		}
		else
		{
			sem_post(&lock);
		}
	}
	pthread_exit(NULL);
}
