#define _POSIX_C_SOURCE 201904L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <primesieve.h>

const size_t NUM_THREADS = 8;
const uint64_t MAX = 1ull << 50;
const uint64_t BUCKET_LEN = 1ull << 22;//512K, my L1 cache is 32K, L2 is 256K, L3 is 6144K
/*  16K: 8.880569s
 *  32K: 6.352548s
 *  64K: 5.235610s
 * 128K: 4.702237s
 * 256K: 4.406385s
 * 512K: 4.293959s
 *   1M: 4.233287s
 *   2M: 4.222358s
 *   4M: 4.282009s
 *   8M: 4.961910s
 */
//2063380

typedef struct{
	uint64_t *ps;
	size_t ps_len;
	uint64_t a;
} Task;

void *worker_cb(void *_arg){
	Task task = *(Task*)_arg;
	uint64_t *ps = task.ps;
	size_t ps_len = task.ps_len;
	uint64_t a = task.a, b = a + BUCKET_LEN;
	uint64_t *bucket = malloc(BUCKET_LEN >> 3);
	uint64_t count = 0;
	for(; b - 1 <= MAX; a += BUCKET_LEN*NUM_THREADS, b = a + BUCKET_LEN){
		memset(bucket, -1, BUCKET_LEN >> 3);
		for(size_t i = 0, p2; i < ps_len; ++i){//TODO: primes p st p2 <= BUCKET_LEN appear in every bucket but those with p2 > BUCKET_LEN do not
			p2 = ps[i];
			for(uint64_t m = (a + p2 - 1)/p2*p2; m < b; m += p2){
				uint64_t i = m - a;
				bucket[i >> 6] &= ~(1ull << (i&0x7F));
			}
		}
		for(size_t i = 0; i < (BUCKET_LEN >> 6); ++i){
			count += __builtin_popcountll(bucket[i]);
		}
	}
	free(bucket);
	return (void*)(uintptr_t)count;
}

int main(){
	size_t ps_len;
	uint64_t *ps = primesieve_generate_primes(0, ceil(sqrt(MAX)), &ps_len, UINT64_PRIMES);
	if(!ps){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory for primes!\e[0m\n");
		exit(EXIT_FAILURE);
	}
	for(size_t i = 0; i < ps_len; ++i){
		ps[i] *= ps[i];
	}
	for(size_t i = 0;; ++i){
		if(ps[i] >= 1 + BUCKET_LEN){
			printf("There are %zu primes that have a multiple in every bucket and %zu others\n", i, ps_len - i);
			break;
		}
	}
	struct timespec start_time, end_time;
	pthread_t tids[NUM_THREADS];
	Task tasks[NUM_THREADS];
	clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
	for(size_t i = 0; i < NUM_THREADS; ++i){
		tasks[i] = (Task){.ps=ps, .ps_len=ps_len, .a= 1 + BUCKET_LEN*i};
		if(pthread_create(tids + i, NULL, worker_cb, tasks + i)){
			fprintf(stderr, "\e[1;31mERROR: Could not start worker threads!\e[0m\n");
			exit(EXIT_FAILURE);
		}
	}
	uint64_t count = 0;
	for(size_t i = 0; i < NUM_THREADS; ++i){
		void *res;
		if(pthread_join(tids[i], &res)){
			fprintf(stderr, "\e[1;31mERROR: Could not join worker threads!\e[0m\n");
			exit(EXIT_FAILURE);
		}
		count += (uintptr_t)res;
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
	primesieve_free(ps);
	printf("Found %"PRIu64" squarefree numbers less than %"PRIu64" in %fs.\n", count, MAX, (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)*0.000000001);
}

