#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

key_t key;
int shmid, size, in = 0, out = 0;

void producer(int n, sem_t* full, sem_t* empty) {
	int *buffer = (int*) shmat(shmid, (void*) 0, 0);
	
	do {
		sleep(1);
		int item = rand() % 100;
		
		sem_wait(empty);
		
		buffer[in] = item;
		printf("Producer produced %d in buffer at %d\n", item, in);
		in = (in + 1) % size;
		
		sem_post(full);
	} while(1);
	
	shmdt(buffer);
}

void consumer(int n, sem_t* full, sem_t* empty) {
	int *buffer = (int*) shmat(shmid, (void*) 0, 0);
	
	do {
		//sleep(2);
		sem_wait(full);
		
		int item = buffer[out];
		buffer[out] = 0;
		printf("Consumer consumed %d from buffer at %d\n", item, out);
		out = (out + 1) % size;
		
		sem_post(empty);
	} while(1);
	
	shmdt(buffer);
}

void main() {
	key = ftok("shmfile", 65);
	shmid = shmget(key, 1024, 0666 | IPC_CREAT);
	
	printf("Enter buffer size: ");
	scanf("%d", &size);
	printf("\n");
	
	sem_unlink("/full");
	sem_unlink("/empty");
	
	sem_t *full = sem_open("/full", O_CREAT, O_RDWR, 0);
	sem_t *empty = sem_open("/empty", O_CREAT, O_RDWR, size);
	
	if(fork() == 0)
		producer(size, full, empty);
	else
		consumer(size, full, empty);
	
	shmctl(shmid, IPC_RMID, NULL);
}
