
#include "config.h"

void * male(void *);    /* Male method declaration. */
void * female(void *);  /* Female method declaration. */

sem_t crit_sem, waiting_male_sem, waiting_female_sem; /* Semaphores used. */

/* Counters used. */
int males_inside = 0; 
int females_inside = 0; 
int males_waiting = 0; 
int females_waiting = 0;
int times_used;
 
/* Number of males and females specified by the user. */
int numMen, numWomen;
char* man = 'm';
char* woman = 'w';

/* Bool to keep record of we should let the other of the same gender in. */
bool men_leaving = false, women_leaving = false;


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
       
    /* Initialize the semaphores used. */
    sem_init(&crit_sem, SHARED, 1);
    sem_init(&waiting_male_sem, SHARED, 0);
    sem_init(&waiting_female_sem, SHARED, 0);

    srand(time(NULL)); /* Seed the randomizer to provide different results. */

    for(i = 0; i < numMen; i++) /* Creates the males. */
    {
        pthread_create(&men, NULL, male, (void*)i);
    }
    for(i = 0; i < numWomen; i++) /* Creates the females. */
    {
        pthread_create(&women, NULL, female, (void*)i);
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

void print(char* gender, int id, int *v)
{
    int visits = (int)v;
    if(gender == 'w')
	printf("🚺 %d enters the bathroom. Visit: " LMAG "%d\n" RESET, id, visits + 1);
    if(gender == 'm') {
	printf("🚹 %d enters the bathroom. Visit: " LBLU "%d\n" RESET, id, visits + 1);
    }
}

void *male(void *arg)
{
	int id = (int)arg;
	int i = 0;
	for (i = 0; i < MAXTIMES; i++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % 4);

		/* Should I wait for bathroom + other stuff */
		sem_wait(&crit_sem);
		if (females_inside > 0 || men_leaving)
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
		print(man, id, i);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(&crit_sem);
		males_inside--;
		men_leaving = true;

		/* Make sure no more men enter while men leave */
		if (males_inside == 0)
			men_leaving = false;

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
		if (males_inside > 0 || women_leaving)
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
        print(woman, id, i);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(&crit_sem);
		females_inside--;

		/* Make sure no more women enter while women leave */
		women_leaving = true;
		if (females_inside == 0)
			women_leaving = false;

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
