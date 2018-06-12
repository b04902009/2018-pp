#include <stdio.h>
#include <thrust/tabulate.h>
#include <thrust/functional.h>
#include <thrust/scan.h>
#include <thrust/execution_policy.h>

template<class T> struct P0{
    const char *base;
    P0(const char *base): base(base) {}
    __host__ __device__ T operator()(const T& index) const { 
        return base[index] == ' ' ? index : -1;
    };
};

template<class T> struct P{
    int32_t *base;
    P(int32_t *base): base(base) {}
    __host__ __device__ T operator()(const T& index) const { 
        return index - base[index];
    };
};


void labeling(const char *cuStr, int *cuPos, int strLen){
	thrust::tabulate(thrust::device, cuPos, cuPos+strLen, P0<int32_t>(cuStr));
	thrust::inclusive_scan(thrust::device, cuPos, cuPos+strLen, cuPos, thrust::maximum<int>());
	thrust::tabulate(thrust::device, cuPos, cuPos+strLen, P<int32_t>(cuPos));
}