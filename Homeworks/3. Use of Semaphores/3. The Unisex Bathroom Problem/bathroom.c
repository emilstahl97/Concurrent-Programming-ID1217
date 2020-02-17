/* The Unisex Bathroom Problem

   features: This program simulates a unisex bathroom used by an arbitrary number of men and women.
             The men and women are represented as threads that work (sleeps) annd uses the bathroom for a random amount of time.
             Synchronisation is done solely with semaphores.

   usage under Linux:
            gcc bathroom.c -o bathroom -lpthread
            ./bathroom numWomen numMen numVisits
*/

#include "config.h"

sem_t lock, waiting_male, waiting_female;

int males_inside = 0; 
int females_inside = 0; 
int males_queue = 0; 
int females_queue = 0;
 
int numMen, numWomen, numVisits;
char* man = 'm', woman = 'w';

bool men_leaving = false, women_leaving = false;

void * male(void *);    
void * female(void *);  
void print(char*, int, int);


int main(int argc, char* argv[]) {
    int i;
    pthread_t men, women;
    
    numWomen = (argc > 1) ? atoi(argv[1]) : MAX_WOMEN;
    numMen = (argc > 2) ? atoi(argv[2]) : MAX_MEN;
    numVisits = (argc > 3) ? atoi(argv[3]) : MAX_VISITS;
       
    sem_init(&lock, SHARED, 1);
    sem_init(&waiting_male, SHARED, 0);
    sem_init(&waiting_female, SHARED, 0);

    for(i = 0; i < numWomen; i++) 
    {
        pthread_create(&women, NULL, female, (void*)i);
    }

    for(i = 0; i < numMen; i++) 
    {
        pthread_create(&men, NULL, male, (void*)i);
    }

    for(i = 0; i < numWomen; i++) 
    {
        pthread_join(women, NULL);
    }

    for(i = 0; i < numMen; i++) 
    {
        pthread_join(men, NULL);
    }

    sem_close(&lock);
    sem_close(&waiting_female);
    sem_close(&waiting_male);

    return 0;

    RESET;
}

void print(char* gender, int id, int v)
{
    int visits = (int)v;
    if(gender == 'w')
	printf("🚺 %d enters the bathroom - Visit: " LMAG "#%d\n" RESET, id, visits + 1);
    if(gender == 'm')
	printf("🚹 %d enters the bathroom - Visit: " LBLU "#%d\n" RESET, id, visits + 1);
    
    // use the bathroom
    sleep(rand() % MAX_TIMEIN);
}

void *male(void *arg)
{
	int visit, id = (int)arg;

	for (visit = 0; visit < numVisits; visit++)
	{
		sleep(rand() % MAX_INTERVAL);

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

		print(man, id, visit);

		sem_wait(&lock);
		males_inside--;
		men_leaving = true;

		if (males_inside == 0)
			men_leaving = false;

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
		sleep(rand() % MAX_INTERVAL);

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

        print(woman, id, visit);

		sem_wait(&lock);
		females_inside--;

		women_leaving = true;
		if (females_inside == 0)
			women_leaving = false;

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