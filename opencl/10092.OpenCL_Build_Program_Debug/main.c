#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <assert.h>
#define MAXN 2048
int main(){
	cl_int status;
	
	/* plarform */
	cl_platform_id platform_id;
	cl_uint platform_num;
	status = clGetPlatformIDs(1, &platform_id, &platform_num);
	assert(status == CL_SUCCESS);

	/* device */
	cl_device_id GPU;
	cl_uint GPU_num;
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &GPU, &GPU_num);
	assert(status == CL_SUCCESS);

	/* getcontext */
	cl_context context = clCreateContext(NULL, 1, &GPU, NULL, NULL, &status);
	assert(status == CL_SUCCESS);

	/* kernelsource */
	char filename[50];
	scanf("%s", filename);
	FILE *kernelfp = fopen(filename, "r");
	assert(kernelfp != NULL);
	
	char buffer[MAXN];
	const char *const_buffer = buffer;
	size_t len = fread(buffer, 1, MAXN, kernelfp);
	cl_program program = clCreateProgramWithSource(context, 1, &const_buffer, &len, &status);
	assert(status == CL_SUCCESS);

	/* program */
	status = clBuildProgram(program, 1, &GPU, NULL, NULL, NULL);
	if(status != CL_SUCCESS){
		/* build information */
		char program_log[MAXN];
		size_t len_ret;
		status = clGetProgramBuildInfo(program, GPU, CL_PROGRAM_BUILD_LOG, MAXN, program_log, &len_ret);
		program_log[len_ret] = '\0';
		printf("%s", program_log);
	}
	return 0;
}