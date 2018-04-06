#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 8
/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
					void *(*start_routine)(void*), void *arg);
// thread 			A pthread_t pointer to the created thread.
// attr 			A pthread_attr_t pointer to the attribute of the thread. (NULL)
// start_routine	Thread starting routine. The routine must have void *routine(void *) prototype.
// arg				A pointer to an optional parameter to the spawned thread/
// return_value		0 if success, non-zero otherwise.

void pthread_exit(void *value);
// value 			A thread can use this pointer to send information back to its parent thread.
// 					(Set to NULL if nothing to send back to main thread.)
*/
void* print(void *thread_id){
	int tid = *((int*)thread_id);
	printf("print: tid = %d\n", tid);
	pthread_exit(NULL);
}
int main(int argc, char *argv[]){
	pthread_t threads[NUM_THREADS];
	int thread_index[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; i++){
		printf("main: create thread %d\n", i);
		thread_index[i] = i;
		int rc = pthread_create(&threads[i], NULL, print, (void *)(thread_index+i));
		if(rc){
			printf("main: error code %d\n", i);
			exit(-1);
		}
	}
	pthread_exit(NULL);
	return 0;
}