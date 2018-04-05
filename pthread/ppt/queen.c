#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>
#define MAXN 20
#define NUM_THREADS 8
int ok(int position[], int next, int test){
	for(int i = 0; i < next; i++) 
		if((position[i] == test) || abs(test-position[i]) == next - i)
			return 0;
	return 1;
}
int queen(int position[], int next, int n){
	if(next >= n)	
		return 1;
	int sum = 0;
	for(int test = 0; test < n; test++)
		if(ok(position, next, test)){
			position[next] = test;
			sum += queen(position, next+1, n);
		}
	return sum;
}
int main(int argc, char *argv[]){
	int position[MAXN];
	assert(argc == 2);
	int n = atoi(argv[1]);
	assert(n < MAXN);
	printf("%d\n", queen(position, 0, n)); 
	return 0;
}