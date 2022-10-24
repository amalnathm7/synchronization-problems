#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT (num + 4) % 5
#define RIGHT (num + 1) % 5

enum {EATING, HUNGRY, THINKING} state[5];
int arr[] = {0, 1, 2, 3, 4};

sem_t phil[5];
//pthread_mutex_t mutex; // USE IF NECESSARY

void test(int num) {
	if (state[num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		state[num] = EATING;
	 	
		sleep(1);
	 	
		printf("Philosopher %d takes chopsticks %d and %d\n",
		              num + 1, num, num + 1);
	 
		printf("Philosopher %d is eating\n", num + 1);
		sem_post(&phil[num]);
	}
}

void take_chopstick(int num) {
	//pthread_mutex_lock(&mutex);
	
	state[num] = HUNGRY;
	
	printf("Philosopher %d is hungry\n", num + 1);
	
	test(num);
	
 	//pthread_mutex_unlock(&mutex);
 	
	sem_wait(&phil[num]);
}

void put_chopstick(int num) {
	//pthread_mutex_lock(&mutex);
	
	state[num] = THINKING;
	
	printf("Philosopher %d puts chopticks %d and %d down\n", num + 1, num, num + 1);
	
	printf("Philosopher %d is thinking\n", num + 1);
	
	test(LEFT);
	
	test(RIGHT);
	
	//pthread_mutex_unlock(&mutex);
}

void* philosopher(void* i) {
	printf("Philosopher %d is thinking\n", *((int*) i) + 1);
			
	while (1) {
		sleep(1);
		
		take_chopstick(*((int*) i));
		
    		sleep(1);
		        
        	put_chopstick(*((int*) i));
	}
	
	// ANOTHER APPROACH
	/* do {
		int i = *(int*) num;
		
		sem_wait(&chopstick[i]);
		sem_wait(&chopstick[(i+1) % 5]);
		
		printf("Philosopher %d is eating\n", i + 1);
		sleep(1);
		
		sem_post(&chopstick[i]);
		sem_post(&chopstick[(i+1) % 5]);
		
		printf("Philosopher %d is thinking\n", i + 1);
		sleep(1);
	} while (1); */
}

void main() {
	printf("There are 5 philosophers and 5 chopsticks\n");
	
	pthread_t tid[5];
	//pthread_mutex_init(&mutex, NULL); 
	
	for (int i=0; i<5; i++)
		sem_init(&phil[i], 0, 0); // sem_init(&chopstick[i], 0, 1); // CASE FOR ANOTHER APPROACH
	
	for (int i=0; i<5; i++)
		pthread_create(&tid[i], NULL, philosopher, &arr[i]);
	
	for (int i=0; i<5; i++)
    		pthread_join(tid[i], NULL);
    	
    	for (int i=0; i<5; i++)
    		sem_destroy(&phil[i]);
    		
    	//pthread_mutex_destroy(&mutex);
}
