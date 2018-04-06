#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>
#define MAXN 20
#define DEBUG
int n, numSolution = 0;
pthread_mutex_t numSolutionLock;
int ok(int position[], int next, int test){
	for(int i = 0; i < next; i++) 
		if((position[i] == test) || abs(test-position[i]) == next - i)
			return 0;
	return 1;
}
void queen(int position[], int next){
	if(next >= n){
		pthread_mutex_lock(&numSolutionLock);
		numSolution++;
		pthread_mutex_unlock(&numSolutionLock);
		return;
	}
	for(int test = 0; test < n; test++)
		if(ok(position, next, test)){
			position[next] = test;
			queen(position, next+1);
		}
}
void* goQueen(void* pos){
	int *position = (int*)pos;
	queen(position, 1);
	pthread_exit(NULL);
}
int main(int argc, char *argv[]){
	assert(argc == 2);
	n = atoi(argv[1]);
	assert(n < MAXN);
	int *position;
	pthread_t threads[MAXN];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&numSolutionLock, NULL);
	for(int i = 0; i < n; i++){
		position = (int*)calloc(n, sizeof(int));
		assert(position != NULL);
		position[0] = i;
		int error = pthread_create(&threads[i], &attr, goQueen, (void*)position);
		assert(error == 0);
	}
	pthread_attr_destroy(&attr);
	for(int i = 0; i < n; i++){
		pthread_join(threads[i], NULL);
#ifdef DEBUG
		printf("main: thread %d done\n", i);
#endif
	}
	printf("total # of solution: %d\n", numSolution);
	pthread_mutex_destroy(&numSolutionLock);
	pthread_exit(NULL);
	return 0;
}