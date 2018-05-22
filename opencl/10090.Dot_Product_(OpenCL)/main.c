#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include <inttypes.h>
#include <CL/cl.h>
#include <assert.h>
#include "utils.h"
#define MAXK 1024
#define MAXN 16777216
#define LOCALTHREADS 256
int main(){
    int N;
    uint32_t key1, key2;
    /* platform */
    cl_int status;
    cl_platform_id platform_id;
    status = clGetPlatformIDs(1, &platform_id, NULL);
    assert(status == CL_SUCCESS);
 
    /* device */
    cl_device_id GPU;
    cl_uint GPU_num;
    status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &GPU, &GPU_num);
    assert(status == CL_SUCCESS);
 
    /* context */
    cl_context context = clCreateContext(NULL, 1, &GPU, NULL, NULL, &status);
    assert(status == CL_SUCCESS);
 
    /* command queue */
    cl_command_queue cq = clCreateCommandQueue(context, GPU, 0, &status);
    assert(status == CL_SUCCESS);
 
    /* kernelsource */
    FILE *fp = fopen("vecdot.cl", "r");
    char buf[MAXK];
    const char *const_buffer = buf;
    size_t len = fread(buf, 1, MAXK, fp);
    cl_program program = clCreateProgramWithSource(context, 1, &const_buffer, &len, &status);
    assert(status == CL_SUCCESS);
 
    /* program */
    status = clBuildProgram(program, 1, &GPU, NULL, NULL, NULL);
    assert(status == CL_SUCCESS);
 
    /* kernel */
    cl_kernel kernel = clCreateKernel(program, "VectorDot", &status);
    assert(status == CL_SUCCESS);
 
    /* vector & buffer */
    cl_uint *C = (cl_uint*)malloc(MAXN/LOCALTHREADS * sizeof(cl_uint));
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, MAXN/LOCALTHREADS * sizeof(cl_uint), C, &status);
    assert(status == CL_SUCCESS);
 
    /* read N, key1, key2 */
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3){
        uint32_t start = (N/LOCALTHREADS)*LOCALTHREADS, end = start+(N%LOCALTHREADS);
        uint32_t plus = 0;
        for(int i = start; i < end; i++)
            plus += encrypt(i, key1) * encrypt(i, key2);
        N = start;
 
        /* set argument */
        status = clSetKernelArg(kernel, 0, sizeof(cl_uint), (void*)&key1);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void*)&key2);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufC);
        assert(status == CL_SUCCESS);
 
        /* setshape*/
        size_t global_threads[] = {(size_t)N};
        size_t local_threads[] = {LOCALTHREADS};
        status = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, global_threads, local_threads, 0, NULL, NULL);
        assert(status == CL_SUCCESS);
 
        /* getcvector*/
        status = clEnqueueReadBuffer(cq, bufC, CL_TRUE, 0, N/LOCALTHREADS * sizeof(cl_uint), C, 0, NULL, NULL);
        assert(status == CL_SUCCESS);
 
        uint32_t sum = 0;
        for (int i = 0; i < N/LOCALTHREADS; i++)
            sum += C[i];
        printf("%" PRIu32 "\n", sum+plus);
    }
 
    /* free*/
    free(C);
    clReleaseContext(context);
    clReleaseCommandQueue(cq);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseMemObject(bufC);
 
    return 0;
}