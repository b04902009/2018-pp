#include "matrix.h"
// #define seq
void multiply(int N, UINT A[][MAXN], UINT B[][MAXN], UINT C[][MAXN]){
#ifndef seq
	UINT tmp;
	for(int i = 0; i < N; i++)
		for(int j = i+1; j < N; j++){
			tmp = B[i][j];
			B[i][j] = B[j][i];
			B[j][i] = tmp;
		}
	pthread_t threads[MAX_THREAD];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(int i = 0; i < MAX_THREAD; i++){
		Argu *argus  = (Argu*)malloc(sizeof(Argu));
		argus->A = (UINT*)A;
		argus->transB = (UINT*)B;
		argus->C = (UINT*)C;
		argus->id = i;
		argus->N = N;
		int error = pthread_create(&threads[i], &attr, divide, (void*)argus);
		assert(!error);
	}
	for(int i = 0; i < MAX_THREAD; i++)
		pthread_join(threads[i], NULL);
#endif

#ifdef seq
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			UINT sum = 0;    // overflow, let it go.
			for(int k = 0; k < N; k++)
				sum += A[i][k] * B[k][j];
			C[i][j] = sum;
		}
	}
#endif
}
void* divide(void* arg){
	Argu *argu = (Argu*)arg;
	UINT *A = argu->A, *transB = argu->transB, *C = argu->C;
	int N = argu->N, id = argu->id, w = (N+MAX_THREAD-1) / MAX_THREAD;
	int s = id*w, e = (id+1)*w;
	for(int i = s; i < e && s < N; i++) {
		for(int j = 0; j < N; j++) {
			UINT sum = 0;    // overflow, let it go.
			for(int k = 0; k < N; k++)
				sum += *((A+i*MAXN)+k) * *((transB+j*MAXN)+k);
			*((C+i*MAXN)+j) = sum;
		}
	}
	pthread_exit(NULL);
}