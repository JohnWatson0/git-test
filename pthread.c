#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"

#define new(ptr, n) ( ptr = (typeof(ptr))malloc(sizeof(typeof(ptr))*n) )

pthread_mutex_t mutex;
sem_t sem;
int sum = 0;
int tmp[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

void* task(void* param) {

	int i, sum_tmp = 0;
	
	for (i = 1; i <= tmp[(long)param]; ++i) {
		sum_tmp += i;
	}
	fprintf(stdout, "[%ld] sum_temp %d\n", (long)param, sum_tmp);
	
	sem_wait(&sem);

	pthread_mutex_lock(&mutex);
	sum += sum_tmp;
	pthread_mutex_unlock(&mutex);

	sem_post(&sem);
}


int main (int argc, char* argv[]) {

	pthread_t* tid = NULL;
	int tid_num = atoi(argv[1]);
	int i;

	if (tid_num != 0) {
		pthread_mutex_init(&mutex, NULL);
		sem_init(&sem, 0, tid_num);
		
		new(tid, tid_num);
		if (tid == NULL) {
			fprintf(stderr, "Malloc err.\n");
			exit(1);
		}
		for (i = 0; i < tid_num; ++i) {
			pthread_create(&tid[i], NULL, task, (void*)(long)i);
		}
		for (i = 0; i < tid_num; ++i) {
			pthread_join(tid[i], NULL);
		}

		fprintf(stdout, "The result is %d\n", sum);
		free(tid);
		sem_destroy(&sem);
		pthread_mutex_destroy(&mutex);
	}

	return 0;
}


