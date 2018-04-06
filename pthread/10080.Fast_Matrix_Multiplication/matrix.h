#include <assert.h>
#include <pthread.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_THREAD 4
#define MAXN 2048
#define UINT unsigned long
typedef struct{
	UINT *A, *transB, *C;
	int id, N;
} Argu;
void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]);
void* divide(void* i);