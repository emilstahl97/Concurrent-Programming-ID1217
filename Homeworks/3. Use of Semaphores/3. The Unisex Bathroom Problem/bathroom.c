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
bool letting_in_people;

/* Number of males and females specified by the user. */
int numMales, numWomen;

/**
 * Main method, initiates all the semaphores and counters and then starts the
 * specified number of males and females. Then joines them together again after
 * the bathroom has been visited MAXTIMES number of times.
 */
int main(int argc, char ** argv)
{
    pthread_t men, women;
    int i; /* Used to iterate over the threads. */
    
    numMales = (argc > 1) ? atoi(argv[1]) : MAX_MEN;
    numWomen = (argc > 2) ? atoi(argv[2]) : MAX_WOMEN;
       
    /* Initialize the semaphores used. */
    sem_init(&crit_sem, SHARED, 1);
    sem_init(&waiting_male_sem, SHARED, 0);
    sem_init(&waiting_female_sem, SHARED, 0);


    letting_in_people = false; /* We are currently not letting people inside. */

    srand(time(NULL)); /* Seed the randomizer to provide different results. */

    for(i = 0; i < numMales; i++) /* Creates the males. */
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

    return EXIT_SUCCESS; /* Once we joined the thread we exit. */
}

/**
 * Prints the current status of the bathroom and its two queues.
 */
void print_status()
{
    printf("\nINSIDE: M=%d, F=%d.\nWAITING: M=%d, F=%d.\n\n", males_inside, females_inside, males_waiting, females_waiting);
}

/**
 * Male and female version is extremely similair (as expected).
 *
 * Male version of the simulation of the Unisex bathroom. This method will try to enter
 * the bathroom until it has been used MAXTIMES number of times.
 *
 * If nobody is inside or only males are inside with no females waiting we enter.
 * Else we have to wait for the next time it will be males turn to enter hence we wait for
 * waiting_male_sem. Once we got this we can safely enter the bathroom. However we might want to
 * let the other males waiting enter as well (if there are any and we are the first male to try
 * to enter the bathroom. Then simulate the usage of the bathroom by sleeping a random amount
 * of time. Then it is time to exit, when we exit and we are the last to exit we pass the baton
 * to a female (if one is waiting). This is to control who gets to enter, and since it is the
 * females turn we pass the baton to the head of the queue for waiting_female_sem.
 */
void * male(void * input)
{
    int i; /* i used for alerting other males. */

    /* Number of males in queue at the moment it is the males turn. */
    int males_in_queue;

    /* Main loop. Only use the bathroom MAXTIMES number of times. */
    while(times_used < MAXTIMES)
    {
        /* We simulate time until we need to go to the bathroom. */
        sleep((rand() % (MAXINTERVAL - MININTERVAL)) + MININTERVAL);

        sem_wait(&crit_sem); /* Critical section began. */
        males_waiting++; /* Increase the number of males waiting to enter. */

        /* Print status. */
        printf("Male trying to enter bathroom.\n");
        print_status();

        /* If we are not allowed to enter. */
        if(females_inside > 0 || females_waiting > 0)
        {
            /* Print status. */
            printf("Males waiting for their turn.\n");
            print_status();

            /* Release mutual exclusion semaphore. */
            sem_post(&crit_sem);
            sem_wait(&waiting_male_sem); /* Waiting for our turn. */

            /*
             * NOTE: We now have the crit_sem because of passing the
             * baton technique. 
             */

            /* Update status to represent us being inside. */
            males_inside++;
            males_waiting--;

            /* If we are responsible of letting rest of the males inside. */
            if(letting_in_people == false)
            {
                letting_in_people = true; /* Only one should do this. */

                /* Number of males we should let in. */
                males_in_queue = males_waiting;

                /* Print status. */
                printf("Males turn, first one entering.\n");
                print_status();

                /* Let the other males in queue inside the bathroom.
                 * We have to accuire the crit_sem every time because
                 * of pass the baton technique. */
                for(i = 0; i < males_in_queue; i++)
                {
                    sem_post(&waiting_male_sem);
                    sem_wait(&crit_sem);
                }
                /* We are done letting males inside. */
                letting_in_people = false;
            }
            else
            {
                /* Print status of us entering. */
                printf("Male entering.\n");
                print_status();
            }

        }
        else /* Males are already inside. */
        {
            males_inside++;
            males_waiting--;

            /* Print status of us entering. */
            printf("Male entering.\n");
            print_status();
        }
        sem_post(&crit_sem); /* We are done with the critical section. */

        /* Simulate going to the toilet. */
        sleep((rand() % (MAXTIMEIN - MINTIMEIN)) + MINTIMEIN);

        /* Now we exit the bathroom. */
        sem_wait(&crit_sem);    /* Begin critical section. */
        times_used++;           /* Increase the counter of times used. */
        males_inside--;         /* We are no longer inside the bathroom. */

        /* Print status to reflect us exiting the toilet. */
        printf("Male exiting bathroom.\n");
        print_status();

        /* If we are the last male to exit the bathroom we want to pass the baton
         * to a female if there is one waiting to enter. */
        if(males_inside == 0)
        {
            printf("Last male has exited bathroom.\n");
            print_status();
            if(females_waiting > 0)
                sem_post(&waiting_female_sem);
            else
                sem_post(&crit_sem);
        }
        else /* Else we exit critical section. */
            sem_post(&crit_sem);
    }
    pthread_exit(NULL);
}

/**
 * Male and female version is extremely similair (as expected).
 *
 * Female version of the simulation of the Unisex bathroom. This method will try to enter
 * the bathroom until it has been used MAXTIMES number of times. 
 * 
 * If nobody is inside or only females are inside with no males waiting we enter.
 * Else we have to wait for the next time it will be females turn to enter hence we wait for
 * waiting_female_sem. Once we got this we can safely enter the bathroom. However we might want to
 * let the other females waiting enter as well (if there are any and we are the first female to try
 * to enter the bathroom. Then simulate the usage of the bathroom by sleeping a random amount
 * of time. Then it is time to exit, when we exit and if we are the last to exit we pass the baton
 * to a male (if one is waiting). This is to control who gets to enter, and since it is the
 * males turn we pass the baton to the head of the queue for waiting_male_sem.
 */
void * female(void * input)
{
    int i; /* i used to iterate over the females to let inside the bathroom. */
    int females_in_queue; /* Number of females to let inside the bathroom. */

    /* Main loop executed a total MAXTIMES number of times between males and females. */
    while(times_used < MAXTIMES)
    {
        /* We simulate time until we need to go to the bathroom. */
        sleep((rand() % (MAXINTERVAL - MININTERVAL)) + MININTERVAL);

        /* Start of the critical section. */
        sem_wait(&crit_sem);

        /* Increase the number of females waiting to enter the bathroom. */
        females_waiting++;

        /* Print status to signal a new female has arrived. */
        printf("Female trying to enter bathroom.\n");
        print_status();

        /* We check if we have to wait for it to be our turn. */
        if(males_inside > 0 || males_waiting > 0)
        {
            /* Print status to show that we are waiting. */
            printf("Females waiting for their turn.\n");
            print_status();

            sem_post(&crit_sem); /* Exit critical section. */
            sem_wait(&waiting_female_sem); /* Waiting for our turn. */

            /*
             * NOTE: We now have the crit_sem because of passing the
             * baton technique. 
             */

            /* Update the status. */
            females_inside++;
            females_waiting--;

            /* If we are the first in line of females to enter we let the
             * rest of the females in the queue know that they can enter as
             * well. */
            if(letting_in_people == false)
            {
                letting_in_people = true; /* Only one female should do this. */

                /* Number of females in the queue at this time. */
                females_in_queue = females_waiting;

                /* Print the status to show that females are now allowed to enter. */
                printf("Females turn, first one entering.\n");
                print_status();

                /* Let the rest of the females in the queue enter.
                 * We pass the baton to every female in queue hence we need to
                 * reaccuire the crit_sem after each female is alerted. */
                for(i = 0; i < females_in_queue; i++)
                {
                    sem_post(&waiting_female_sem);
                    sem_wait(&crit_sem);
                }
                /* We are now done letting females inside. */
                letting_in_people = false;
            }
            /* We are free to enter. */
            else
            {
                printf("Female entering.\n");
                print_status();
            }
        }
        else /* Females are already inside. */
        {
            /* Update counters and print status to reflect this change. */
            females_inside++;
            females_waiting--;
            printf("Female entering.\n");
            print_status();
        }
        sem_post(&crit_sem); /* End of critical section. */

        /* Simulate going to the toilet. */
        sleep((rand() % (MAXTIMEIN - MINTIMEIN)) + MINTIMEIN);

        sem_wait(&crit_sem);    /* Start of critical section to exit bathroom. */
        times_used++;           /* Increas the number of times 
                                   the bathroom has been used. */
        females_inside--;       /* We are no longer inside the bathroom. */

        /* Prints the status to show that we exited. */
        printf("Female exiting bathroom.\n");
        print_status();

        /* If we are the last female to exit the bathroom we let males enter if there
         * are any in the queue. */
        if(females_inside == 0)
        {
            /* Prints status to show we are the last female. */
            printf("Last female has exited bathroom.\n");
            print_status();

            /* If there are any males waiting we pass the baton else just exit 
             * critical seciton. */
            if(males_waiting > 0)
                sem_post(&waiting_male_sem);
            else
                sem_post(&crit_sem);
        }
        /* If there are more females inside we can safely exit the critical section. */
        else
            sem_post(&crit_sem);
    }
    pthread_exit(NULL);
}