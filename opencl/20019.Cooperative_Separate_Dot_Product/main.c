#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include <inttypes.h>
#include <CL/cl.h>
#include <assert.h>
#define MAXK 2048
#define MAXN 1073741824
#define CHUNK 1024
#define LOCALSIZE 256
#define MAXGPU 10
#define DEVICENUM 2
int main(){
    int N;
    uint32_t key1, key2;
    /* platform */
    cl_int status;
    cl_platform_id platform_id;
    status = clGetPlatformIDs(1, &platform_id, NULL);
    assert(status == CL_SUCCESS);
 
    /* device */
    cl_device_id GPU[MAXGPU];
    cl_uint GPU_num;
    status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_num);
    assert(status == CL_SUCCESS);
 
    /* context */
    cl_context context = clCreateContext(NULL, DEVICENUM, GPU, NULL, NULL, &status);
    assert(status == CL_SUCCESS);
 
    /* command queue */
    cl_command_queue cq[DEVICENUM];
    for(int device = 0; device < DEVICENUM; device++){
        cq[device] = clCreateCommandQueue(context, GPU[device], CL_QUEUE_PROFILING_ENABLE, &status);
        assert(status == CL_SUCCESS);
    }
 
    /* kernelsource */
    FILE *fp = fopen("vecdot.cl", "r");
    char buf[MAXK];
    const char *const_buffer = buf;
    size_t len = fread(buf, 1, MAXK, fp);
    cl_program program = clCreateProgramWithSource(context, 1, &const_buffer, &len, &status);
 
    /* build program */
    status = clBuildProgram(program, DEVICENUM, GPU, NULL, NULL, NULL);
    if(status != CL_SUCCESS){
        char log[MAXK];
        size_t logLength;
        for(int device = 0; device < DEVICENUM; device++){
            clGetProgramBuildInfo(program, GPU[device], CL_PROGRAM_BUILD_LOG, MAXK, log, &logLength);
            puts(log);
        }
        exit(-1);
    }
 
    /* create kernel */
    cl_kernel kernel = clCreateKernel(program, "VectorDot", &status);
 
    /* vector & buffer */
    int size = ((MAXN+CHUNK-1)/CHUNK+LOCALSIZE-1)/LOCALSIZE;
    // cl_uint *C = (cl_uint*)malloc(size * sizeof(cl_uint));
    cl_mem bufC[DEVICENUM];
    for(int device = 0; device < DEVICENUM; device++){
        bufC[device] = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size * sizeof(cl_uint), 0, &status);
        assert(status == CL_SUCCESS);
    }
 
    cl_uint C[DEVICENUM][size];
 
    /* read N, key1, key2 */
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3){
        uint32_t sum[DEVICENUM] = {0};
        /* setshape*/
        int originN = N;
        int tmp = CHUNK * LOCALSIZE * DEVICENUM;
        N += tmp - N%tmp;
        size_t global_threads[] = {(size_t)N/CHUNK/DEVICENUM};
        size_t local_threads[] = {(size_t)LOCALSIZE};
 
    // #pragma omp parallel for
        for(int device = 0; device < DEVICENUM; device++){
            int s = N / DEVICENUM * device;
            int e = device == DEVICENUM-1 ? originN : N/DEVICENUM * (device+1);
            /* set argument */
            status = clSetKernelArg(kernel, 0, sizeof(cl_uint), (void*)&key1);
            status = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void*)&key2);
            status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufC[device]);
            status = clSetKernelArg(kernel, 3, sizeof(int), (void*)&s);
            status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&e);
            status = clEnqueueNDRangeKernel(cq[device], kernel, 1, NULL, global_threads, local_threads, 0, NULL, NULL);
        }
        /* getcvector*/
        int n = N / CHUNK / LOCALSIZE / DEVICENUM;
        for(int device = 0; device < DEVICENUM; device++){
            status = clEnqueueReadBuffer(cq[device], bufC[device], CL_TRUE, 0, 
                                         n * sizeof(cl_uint), C[device], 0, NULL, NULL);
            for(int i = 0; i < n; i++)
                sum[device] += C[device][i];
        }
        printf("%" PRIu32 "\n", sum[0]+sum[1]);
    }
 
    /* free*/
    // free(C);
    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    for(int device = 0; device < DEVICENUM; device++){
        clReleaseCommandQueue(cq[device]);
        clReleaseMemObject(bufC[device]);
    }
 
    return 0;
}