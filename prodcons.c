#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int size, in = 0, out = 0;
int* buffer;
sem_t full, empty;
pthread_mutex_t mutex;
pthread_t ptid, ctid;

void producer() {
	do {
		//sleep(1); // For better output view
		int item = rand() % 100;
		
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		
		buffer[in] = item;
		printf("Producer produced %d in buffer at %d\n", item, in);
		in = (in + 1) % size;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	} while (1);
}

void consumer() {
	do {
		//sleep(2); // For keeping different rates of production and consumption
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		
		int item = buffer[out];
		buffer[out] = 0;
		printf("Consumer consumed %d from buffer at %d\n", item, out);
		out = (out + 1) % size;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	} while (1);
}

void main() {
	printf("Enter buffer size: ");
	scanf("%d", &size);
	printf("\n");
	
	buffer = (int*) malloc(size * sizeof(int));
	
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, size);
	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&ptid, NULL, (void*) producer, NULL);
    	pthread_create(&ctid, NULL, (void*) consumer, NULL);
    	pthread_join(ptid, NULL);
    	pthread_join(ctid, NULL);
    	
    	sem_destroy(&full);
    	sem_destroy(&empty);
    	pthread_mutex_destroy(&mutex);
    	free(buffer);
}
