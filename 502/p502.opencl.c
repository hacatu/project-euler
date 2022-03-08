#define CL_TARGET_OPENCL_VERSION 220

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <CL/cl.h>

static cl_context context;
static cl_command_queue queue;
static cl_kernel kernel_mat_assign, kernel_mat_eye, kernel_mat_mod_mul;

int check_opencl_err(cl_int err, const char *fmt, int die){
	if(err != CL_SUCCESS){
		char *what = "UNKNOWN";
		switch(err){
		case CL_INVALID_PROGRAM_EXECUTABLE:
			what = "CL_INVALID_PROGRAM_EXECUTABLE";
			break;
		case CL_INVALID_COMMAND_QUEUE:
			what = "CL_INVALID_COMMAND_QUEUE";
			break;
		case CL_INVALID_KERNEL:
			what = "CL_INVALID_KERNEL";
			break;
		case CL_INVALID_CONTEXT:
			what = "CL_INVALID_CONTEXT";
			break;
		case CL_INVALID_KERNEL_ARGS:
			what = "CL_INVALID_KERNEL_ARGS";
			break;
		case CL_INVALID_WORK_DIMENSION:
			what = "CL_INVALID_WORK_DIMENSION";
			break;
		case CL_INVALID_GLOBAL_WORK_SIZE:
			what = "CL_INVALID_GLOBAL_WORK_SIZE";
			break;
		case CL_INVALID_WORK_GROUP_SIZE:
			what = "CL_INVALID_WORK_GROUP_SIZE";
			break;
		case CL_INVALID_WORK_ITEM_SIZE:
			what = "CL_INVALID_WORK_ITEM_SIZE";
			break;
		case CL_INVALID_GLOBAL_OFFSET:
			what = "CL_INVALID_GLOBAL_OFFSET";
			break;
		}
		fprintf(stderr, fmt, err, what);
		if(die){
			exit(EXIT_FAILURE);
		}
	}
	return err != CL_SUCCESS;
}

//put input matrix in S, output will be in A, S, or B (the return value will be the buffer holding the output)
cl_mem mat_mod_pow_handler(int64_t n, cl_mem _bufA, cl_mem _bufS, cl_mem _bufB, int64_t e, int64_t p, int64_t c){
	cl_mem bufA = _bufA, bufS = _bufS, bufB = _bufB, bufT;
	//mat_assign(n, A, S);
	if(n&1){
		//call kernel
		mat_assign(n, bufS, bufB);
	}else{
		//call kernel
		mat_eye(n, bufB);
	}
	while(e >>= 1){
		//call kernel
		mat_mod_mul(n, bufS, bufS, bufA, p, c);
		bufT = bufA;//A was our dead variable but now S is so exchange their names
		bufA = bufS;
		bufS = bufT;//now A is dead again
		if(e&1){
			//call kernel
			mat_mod_mul(n, bufB, bufS, bufA, p, c);
			bufT = bufA;//A was our dead variable but now B is so exchange their names
			bufA = bufB;
			bufB = bufT;//now A is dead again
		}
	}
	return B;
}

int64_t f_horiz(int64_t w, int64_t h, int64_t p, int64_t c){
	
	size_t n = 2*h;
	
	int64_t *A = calloc(n*n, sizeof(int64_t));
	int64_t *B = malloc(n*n*sizeof(int64_t));
	int64_t *C = malloc(n*n*sizeof(int64_t));
	
	cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_WRITE, n*n*sizeof(int64_t), NULL, NULL);
	cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_WRITE, n*n*sizeof(int64_t), NULL, NULL);
	cl_mem bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, n*n*sizeof(int64_t), NULL, NULL);
	
	for(size_t r = 0; r < h; ++r){
		for(size_t i = 0; i < r; ++i){
			if((i&1) == (r&1)){
				A[r*n + i] = 1;     //f(w+1, h, r) = ... + f(w, h, i) + ... if i = r mod 2
				A[(r+h)*n + i+h] = 1; //g(w+1, h, r) = ... + g(w, h, i) + ... if i = r mod 2
			}else{
				A[r*n + i+h] = 1;   //f(w+1, h, r) = ... + g(w, h, i) + ... if i != r mod 2
				A[(r+h) + i] = 1;   //g(w+1, h, r) = ... + f(w, h, i) + ... if i != r mod 2
			}
		}
		for(size_t i = r; i < h; ++i){
			A[r*n + i] = 1;         //f(w+1, h, r) = ... + f(w, h, i) + ...
			A[(r+h)*n + i+h] = 1;     //g(w+1, h, r) = ... + g(w, h, i) + ...
		}
	}
	
	clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, n*n*sizeof(int64_t), A, 0, NULL, NULL);
	//clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, n*n*sizeof(int64_t), B, 0, NULL, NULL);
	//clEnqueueWriteBuffer(queue, bufC, CL_TRUE, 0, n*n*sizeof(int64_t), C, 0, NULL, NULL);
	
	clSetKernelArg(kernel, 0, sizeof(int32_t), &n);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufA);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &bufC);
	clSetKernelArg(kernel, 4, sizeof(int64_t), &w);
	clSetKernelArg(kernel, 5, sizeof(int64_t), &p);
	clSetKernelArg(kernel, 6, sizeof(int32_t), &c);
	
	puts(">>> Ready to launch OpenCL kernel");
	
	size_t local_dims[2] = {n, n};
	size_t global_dims[2] = {n, n};
	cl_event event = NULL;
	cl_int err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_dims, local_dims, 0, NULL, &event);
	clWaitForEvents(1, &event);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to run kernel (err=%d): %s\e[0m\n", 1);
	puts(">>> Finished");
	
	clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, n*n*sizeof(int64_t), C, 0, NULL, NULL);
	
	clReleaseMemObject(bufA);
	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	
	free(A);
	free(B);
	
	int64_t res = 0;
	for(size_t b = 0; b < h; b += c){
		for(size_t r = b; r < b + c && r < h; ++r){
			res += C[r*n + h];
		}
		res %= p;
	}
	free(C);
	return res;
}

int64_t F_horiz(int64_t w, int64_t h, int64_t p, int64_t c){
	int64_t ret = f_horiz(w, h, p, c) - f_horiz(w, h - 1, p, c);
	ret %= p;
	if(ret < 0){
		ret += p;
	}
	return ret;
}

int main(){
	puts(">>> Starting OpenCL...");
	cl_int err;
	cl_uint num_platforms;
	err = clGetPlatformIDs(0, NULL, &num_platforms);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get num platform ids (err=%d): %s\e[0m\n", 1);
	cl_platform_id *platform_ids = malloc(num_platforms*sizeof(cl_platform_id));
	err = clGetPlatformIDs(num_platforms, platform_ids, NULL);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get platform ids list (err=%d): %s\e[0m\n", 1);
	char name_buf[1024] = {};
	fprintf(stderr, "\e[1;33mFound %u platforms:\e[0m\n", num_platforms);
	for(size_t i = 0; i < num_platforms; ++i){
		err = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_PROFILE, 1024, name_buf, NULL);
		fprintf(stderr, "%zu:CL_PLATFORM_PROFILE%s\n", i, name_buf);
		err = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VERSION, 1024, name_buf, NULL);
		fprintf(stderr, "%zu:CL_PLATFORM_VERSION%s\n", i, name_buf);
	}
	fprintf(stderr, "----------\n");
	
	cl_uint num_devices;
	err = clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get num device ids (err=%d): %s\e[0m\n", 1);
	cl_device_id *device_ids = malloc(num_devices*sizeof(cl_device_id));
	err = clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_GPU, num_devices, device_ids, NULL);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get num device ids (err=%d): %s\e[0m\n", 1);
	fprintf(stderr, "\e[1;33mFound %u devices:\e[0m\n", num_platforms);
	for(size_t i = 0; i < num_devices; ++i){
		err = clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, 1024, name_buf, NULL);
		fprintf(stderr, "%zu:CL_DEVICE_NAME: %s\n", i, name_buf);
	}
	fprintf(stderr, "----------\n");
	context = clCreateContext(NULL, 1, &device_ids[0], NULL, NULL, NULL);
	queue = clCreateCommandQueueWithProperties(context, device_ids[0], NULL, &err);
	
	int cl_source_fd = open("modmats.cl", O_RDONLY);
	if(cl_source_fd < 0){
		fprintf(stderr, "\e[1;31mERROR: could not open opencl source file: %s\e[0m\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct stat stat_buf;
	fstat(cl_source_fd, &stat_buf);
	char *kernel_source = malloc(stat_buf.st_size + 1);
	read(cl_source_fd, kernel_source, stat_buf.st_size);
	close(cl_source_fd);
	kernel_source[stat_buf.st_size] = '\0';
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernel_source, NULL, &err);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to compile kernel (err=%d): %s\e[0m\n", 1);
	err = clBuildProgram(program, 0, NULL, "", NULL, NULL);
	int built = !check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to build kernel (err=%d): %s\nContinuing in order to show errors...\e[0m\n", 0);
	free(kernel_source);
	
	size_t log_size;
	err = clGetProgramBuildInfo(program, device_ids[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	!check_opencl_err(err, "\e[1;31mERROR: clGetProgramBuildInfo failed getting size (err=%d): %s\e[0m\n", 1);
	char *messages = malloc((log_size + 1)*sizeof(char));
	err = clGetProgramBuildInfo(program, device_ids[0], CL_PROGRAM_BUILD_LOG, log_size, messages, NULL);
	check_opencl_err(err, "\e[1;31mERROR: clGetProgramBuildInfo failed getting log (err=%d): %s\e[0m\n", 1);
	if(log_size > 0){
		messages[log_size] = '\0';
		printf(">>> OpenCL Compiler message: %s\n", messages);
	}
	if(!built){
		puts("End of messages.  Build failed so the program will now stop.");
		exit(EXIT_FAILURE);
	}
	free(messages);
	
	kernel_mat_assign = clCreateKernel(program, "mat_assign", &err);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get kernel entry point 'mat_assign' (err=%d): %s\e[0m\n", 1);
	kernel_mat_eye = clCreateKernel(program, "mat_eye", &err);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get kernel entry point 'mat_eye' (err=%d): %s\e[0m\n", 1);
	kernel_mat_mod_mul = clCreateKernel(program, "mat_mod_mul", &err);
	!check_opencl_err(err, "\e[1;31mERROR: OpenCL failed to get kernel entry point 'mat_mod_mul' (err=%d): %s\e[0m\n", 1);
	
	int64_t w = 100, h = 100, p = 1000000007;
	int64_t c = ((1ull<<63) - 1)/(p*p) - 1;
	if(c <= 1){
		fprintf(stderr, "\e[1;31mERROR: modulus is too large, cannot prevent overflow\e[0m\n");
		exit(EXIT_FAILURE);
	}
	
	printf("%%%%%% F(%"PRId64", %"PRId64")%%%"PRId64"=%"PRId64"\n", w, h, p, F_horiz(w, h, p, c));
	
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseProgram(program);
	clReleaseKernel(kernel_mat_assign);
	clReleaseKernel(kernel_mat_eye);
	clReleaseKernel(kernel_mat_mod_mul);
}

