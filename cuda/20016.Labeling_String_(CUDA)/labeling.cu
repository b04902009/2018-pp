#include <stdio.h>
#define BLOCKSIZE 512
__global__ void prefix(const char *cuStr, int *cuPos, int strLen){
    int localIdx = threadIdx.x;
    int globalIdx = blockIdx.x * blockDim.x + threadIdx.x;
    if(globalIdx >= strLen) return;

    __shared__ int buf[BLOCKSIZE];
    buf[localIdx] = cuStr[globalIdx] == ' ' ? localIdx : -1;
    __syncthreads();

    int cnt = 0;
    while(buf[localIdx] < 0 && localIdx > cnt){
        cnt++;
        buf[localIdx] = buf[localIdx-cnt];
    }
    cuPos[globalIdx] = localIdx - buf[localIdx];
}
__global__ void combine(int *cuPos, int strLen){
    int localIdx = threadIdx.x;
    int globalIdx = blockIdx.x * blockDim.x + threadIdx.x;
    if(globalIdx >= strLen) return;

    if(blockIdx.x > 0 && cuPos[globalIdx] == localIdx + 1)
        cuPos[globalIdx] += cuPos[blockIdx.x * blockDim.x - 1];
}
void labeling(const char *cuStr, int *cuPos, int strLen){
    const int GRIDSIZE = (strLen+BLOCKSIZE) / BLOCKSIZE;
    prefix <<< GRIDSIZE, BLOCKSIZE >>> (cuStr, cuPos, strLen);
    combine <<< GRIDSIZE, BLOCKSIZE >>> (cuPos, strLen);
}