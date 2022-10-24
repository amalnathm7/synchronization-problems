#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

sem_t wrt;
pthread_mutex_t mutex;
int readcount = 0;
int buffer;
int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void* reader(void* num) {
	while(1) {
		pthread_mutex_lock(&mutex);
		
		readcount++;
		
		if(readcount == 1)
			sem_wait(&wrt);
			
		pthread_mutex_unlock(&mutex);
		
		sleep(1);
		printf("Reader %d: %d\n", *((int*) num), buffer);
		
		pthread_mutex_lock(&mutex);
		
		readcount--;
		
		if(readcount == 2)
			sem_post(&wrt);
		
		pthread_mutex_unlock(&mutex);
	}
}

void* writer(void* num) {
	while(1) {
		sem_wait(&wrt);
		
		sleep(1);
		buffer = rand() % 100;
		printf("Writer %d: %d\n", *((int*) num), buffer);
		
		sem_post(&wrt);
	}
}

void main() {
	sem_init(&wrt, 0, 1);
	pthread_mutex_init(&mutex, NULL);

	pthread_t rtid[3], wtid[3];
	
	for(int i=0; i<3; i++)
		pthread_create(&rtid[i], NULL, reader, &a[i]);
	for(int i=0; i<3; i++)
		pthread_create(&wtid[i], NULL, writer, &a[i]);
	
	for(int i=0; i<3; i++)
		pthread_join(rtid[i], NULL);
	for(int i=0; i<3; i++)
		pthread_join(wtid[i], NULL);

    	pthread_mutex_destroy(&mutex);
    	sem_destroy(&wrt);
}
