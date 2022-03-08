#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const size_t NUM_THREADS = 8;
const uint64_t N = 1000000000000ull;
const uint64_t pow10[] = {1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull, 10000000ull, 100000000ull, 1000000000ull, 10000000000ull, 100000000000ull};

int is_sq_s_num(uint64_t n, uint64_t l){
	//l is the 10^k lower bound of n*n, so t is 2^k,
	//then m is a mask of which locations to split n*n at
	uint64_t t = 1ull << __builtin_ctzll(l);
	for(uint64_t m = 1; m < t; ++m){
		//printf("mask %"PRIu64"\n", m);
		uint64_t m2 = m + t;
		uint64_t n2 = n*n;
		uint64_t s = 0;
		while(m2 >= 1){
			uint64_t p = n2%pow10[__builtin_ctzll(m2) + 1];
			//printf(" part %"PRIu64"\n", p);
			s += p;
			n2 /= pow10[__builtin_ctzll(m2) + 1];
			m2 >>= __builtin_ctzll(m2) + 1;
		}
		if(s == n){
			return 1;
		}
	}
	return 0;
}

void *worker_cb(void *_a){
	uint64_t s = 0;
	for(uint64_t n = (uintptr_t)_a, l = 10; n*n <= N; n += (9*NUM_THREADS - 1)){
		if(n*n >= 10*l){
			l *= 10;
		}
		if(is_sq_s_num(n, l)){
			s += n*n;
			//printf("%"PRIu64"\n", n*n);
		}
		++n;
		if(n*n >= 10*l){
			l *= 10;
		}
		if(is_sq_s_num(n, l)){
			s += n*n;
			//printf("%"PRIu64"\n", n*n);
		}
	}
	return (void*)(uintptr_t)s;
}

int main(){
	pthread_t tids[NUM_THREADS];
	for(size_t i = 0; i < NUM_THREADS; ++i){
		if(pthread_create(tids + i, NULL, worker_cb, (void*)(uintptr_t)(9*(i + 1)))){
			fprintf(stderr, "\e[1;31mERROR: Could not start worker threads!\e[0m\n");
			exit(EXIT_FAILURE);
		}
	}
	uint64_t s = 0;
	for(size_t i = 0; i < NUM_THREADS; ++i){
		void *res;
		if(pthread_join(tids[i], &res)){
			fprintf(stderr, "\e[1;31mERROR: Could not join worker threads!\e[0m\n");
			exit(EXIT_FAILURE);
		}
		s += (uintptr_t)res;
	}
	printf("%"PRIu64"\n", s);
}

