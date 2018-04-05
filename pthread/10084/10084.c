#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include "utils.h"
#define MAXN 10000005
#define MAX_THREAD 6

uint32_t a[MAXN];
uint32_t sum[MAX_THREAD+1];
int n, p;
uint32_t key;

void* prefix_sum(void *idnum){
    int id = *(int*)idnum;
    int w = n / p;
    int tmp = 0;
    for(int j = 1; j <= w; j++){
        tmp += encrypt(w*id+j, key);
        a[w*id+j] = tmp;
    }
    sum[id+1] = a[w*(id+1)];
    pthread_exit(NULL);
}
void* patch(void *idnum){
    int id = *(int*)idnum;
    int w = n / p;
    for(int j = 1; j <= w; j++)
        a[w*id+j] += sum[id];
    pthread_exit(NULL);
}
int main() {
// CPU setting: CPU0 ~ CPU5
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        for (int i = 0; i < 6; i++)
                CPU_SET(i, &cpuset);
        assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
    }

    pthread_t threads[MAX_THREAD];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int thread_index[MAX_THREAD];

    while (scanf("%d %" PRIu32, &n, &key) == 2){
        // Partition data among p processors and each processor computes its prefix sum.
        p = (n < MAX_THREAD) ? n : MAX_THREAD;
        for(int i = 0; i < p; i++){ 
            thread_index[i] = i;
            int error = pthread_create(&threads[i], &attr, prefix_sum, (void*)(thread_index+i));
            assert(error == 0);
        }
        for(int i = 0; i < p; i++)
            pthread_join(threads[i], NULL);

        // Compute the prefix sum of the last elements from all processors.
        for(int i = 1; i <= p; i++)
            sum[i] += sum[i-1];

        // Use the prefix sum from the last elements to patch up the answers
        for(int i = 0; i < p; i++){ 
            int error = pthread_create(&threads[i], &attr, patch, (void*)(thread_index+i));
            assert(error == 0);
        }
        for(int i = 0; i < p; i++)
            pthread_join(threads[i], NULL);
        if((n/p)*p != n)
            for(int i = (n/p)*p+1; i <= n; i++)
                a[i] = a[i-1] + encrypt(i, key);
        output(a, n);
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}
