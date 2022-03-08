#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <pthread.h>

#define MAX_PRIME_FACTORS 7

typedef struct{
	uint64_t num_primes;
	uint64_t primes[MAX_PRIME_FACTORS];
} factors_t;

typedef struct{
	uint64_t num_primes;
	uint64_t *primes;
	uint64_t bucket_len;
	factors_t *bucket;
	uint64_t num_threads;
	pthread_t tid;
	uint64_t n_max;
	uint64_t n_offset;
	uint64_t result;
} task_t;

uint64_t *sieve_factors_first_bucket(uint64_t bucket_len, factors_t first_bucket[restrict static bucket_len], uint64_t *_num_primes){
	uint64_t *primes = malloc((size_t)(1.25506*bucket_len/log(bucket_len) - 1)*sizeof(uint64_t));
	if(!primes){
		return NULL;
	}
	primes[0] = 3;
	uint64_t num_primes = 1;
	for(uint64_t m = 3; m <= bucket_len; m += 3){
		first_bucket[m - 1].num_primes++;
		first_bucket[m - 1].primes[0] = 3;
	}
	for(uint64_t b = 5; b <= 7; b += 2){
		for(uint64_t p = b; p <= bucket_len; p += 6){
			if(first_bucket[p - 1].num_primes){
				continue;
			}
			for(uint64_t m = p; m <= bucket_len; m += p){
				first_bucket[m - 1].primes[first_bucket[m - 1].num_primes++] = p;
			}
		}
	}
	*_num_primes = num_primes;
	return realloc(primes, num_primes*sizeof(uint64_t)) ?: primes;
}

uint64_t sieve_factors_reduce_bucket(uint64_t bucket_len, factors_t *bucket, uint64_t n_offset){
	uint64_t factors[MAX_PRIME_FACTORS + 1];
	for(uint64_t m = n_offset + 1; m <= n_offset + bucket_len; ++m){
		uint64_t q = m;
		uint64_t tz = __builtin_ctzll(m);
		uint64_t num_factors = !!tz;
		if(tz){
			factors[0] = 1ull << tz;
			q >>= tz;
		}
		for(uint64_t i = 0; i < bucket[m - n_offset - 1].num_primes; ++i){
			uint64_t p = bucket[m - n_offset - 1].primes[i];
			factors[num_factors] = 1;
			do{
				factors[num_factors] *= p;
				q /= p;
			}while(q%p == 0);
			++num_factors;
		}
		if(q != 1){
			factors[num_factors++] = q;
		}
		if(num_primes == 1){
			if(tz >= 3){
				return (factors[0] >> 1) + 1;
			}else{
				return 1;
			}
		}else if(num_primes == 2){
			if(tz == 1){
				return 1;
			}else if(tz >= 3){
				//m = 2^tz*q -> 2^(tz-1)-1,2^(tz-1)+1, 2^tz-1 mod 2^tz, 1 mod q
				uint64_t p = factors[0], q = factors[1];
				uint64_t r = lift_crt((p >> 1) - 1, p, 1, q);
				uint64_t r_max = p*q - r > r ? p*q - r : r;
				r = lift_crt((p >> 1) + 1, p, 1, q);
				r = p*q - r > r ? p*q - r : r;
				r_max = r > r_max ? r : r_max;
				r = lift_crt(p - 1, p, 1, q);
				r = p*q - r > r ? p*q - r : r;
				return r > r_max ? r : r_max;
			}else{
				//m = p*q -> 1 mod p, q-1 mod q
				uint64_t r = lift_crt(1, factors[0], factors[1] - 1, factors[1]);
				return p*q - r > r ? p*q - r : r;
			}
		}else{
			//beep boop bop beep
		}
	}
}

void *sieve_factors_worker(void *_task){
	task_t *task = _task;
	uint64_t max = task->n_max;
	factors_t *bucket = task->bucket;
	for(uint64_t n_offset = task->n_offset; n_offset <= max; n_offset += task->bucket_len*task->num_threads){
		uint64_t bucket_len = task->bucket_len <= max - n_offset ? task->bucket_len : max - n_offset;
		for(uint64_t i = 0; i < bucket_len; ++i){
			bucket[i].num_primes = 0;
		}
		for(uint64_t i = 0; i < task->num_primes; ++i){
			uint64_t p = task->primes[i];
			//need the first multiple of p, pq >= n_offset + 1 --> q >= (n_offset + 1)/p --> q = (n_offset + p)/p
			for(uint64_t j = (n_offset + p)/p*p - n_offset - 1; j < bucket_len; j += p){
				bucket[j].primes[bucket[j].num_primes++] = p;
			}
		}
		task->result += sieve_factors_reduce_bucket(bucket_len, bucket, n_offset);
	}
	return _task;
}

void sieve_factors(uint64_t max){
	uint64_t bucket_len;
	for(bucket_len = sqrt(max); bucket_len*bucket_len < max; ++bucket_len);
	factors_t *first_bucket = calloc(bucket_len, sizeof(factors_t));
	if(!first_bucket){
		return 0;
	}
	uint64_t num_primes, *primes = sieve_factors_first_bucket(bucket_len, first_bucket, &num_primes);
	if(!primes){
		free(first_bucket);
		return 0;
	}
	uint64_t num_threads = sysconf(_SC_NPROCESSORS_ONLN);
	task_t tasks[num_threads];
	tasks[0] = (task_t){
		.num_primes=num_primes,
		.primes=primes,
		.bucket_len=bucket_len,
		.bucket=NULL,
		.num_threads=num_threads,
		.tid=0,
		.n_max=max,
		.n_offset=bucket_len,
		.result=0
	};
	for(uint64_t i = 0; i < num_threads; ++i){
		task[i].bucket = malloc(bucket_len*sizeof(factors_t));
		if(!bucket){
			for(uint64_t j = 0; j < i; ++j){
				free(task[j].bucket);
			}
			free(primes);
			free(first_bucket);
			return 0;
		}
	}
	for(uint64_t i = 1; i < num_threads; ++i){
		tasks[i] = (task_t){
			num_primes,
			primes,
			bucket_len,
			bucket,
			num_threads,
			0,
			max,
			bucket_len*(i + 1),
			0
		}
		int err = pthread_create(&tasks[i].tid, NULL, sieve_factors_worker, tasks + i);
		if(err){
			for(uint64_t j = 0; j < num_threads; ++j){
				free(task[j].bucket);
			}
			free(primes);
			free(first_bucket);
			return 0;
		}
	}
	sieve_factors_worker(tasks);
	for(uint64_t i = 1; i < num_threads; ++i){
		pthread_join(tasks[i].tid, NULL);
	}
	for(uint64_t j = 0; j < num_threads; ++j){
		free(task[j].bucket);
	}
	free(primes);
	free(first_bucket);
}

