#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <primesieve.h>

static const uint64_t N = 1000000;
static const uint64_t pows3[13] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441};

static inline void visit_partition(uint64_t N, uint8_t flags[static N], uint64_t q){
	flags[q] >>= 1;
}

/* N is the (exclusive) upper bound on the numbers to count partitions of
 * flags is an array whose elements start as 3.  Every integer from 0 (inclusive) to N (exclusive) has a flag in flags.
 *     when a partition of q < N is found, the flag of q is shifted to the right, so if it has two ways of being partitioned it will be zero
 * acc is the sum of the terms in a partition being built recursively
 * lo is the minimum (inclusive) 3 exponent and hi is the maximum (inclusive) 2 exponent.
 */
static inline void make_partitions_rec(uint64_t N, uint8_t flags[static N], uint64_t acc, uint64_t lo, uint64_t hi){
	if(acc >= N){
		return;
	}
	for(uint64_t i = lo; i < 13; ++i){
		if(acc + pows3[i] >= N){
			break;
		}
		for(uint64_t j = 0; j <= hi; ++j){
			if(acc + (pows3[i] << j) >= N){
				break;
			}
			if(j){
				make_partitions_rec(N, flags, acc + (pows3[i] << j), i + 1, j - 1);
			}else{
				visit_partition(N, flags, acc + (pows3[i] << j));
			}
		}
	}
}

static uint8_t *make_partitions(uint64_t N){
	uint8_t *flags = malloc(N*sizeof(uint8_t));
	if(!flags){
		return NULL;
	}
	memset(flags, 3, N*sizeof(uint8_t));
	make_partitions_rec(N, flags, 0, 0, 63 - __builtin_clzll(N));
	return flags;
}

static uint64_t sum_unipartition_primes(uint64_t N){
	size_t primes_len;
	uint64_t *primes = primesieve_generate_primes(0, N, &primes_len, UINT64_PRIMES);
	uint8_t *flags = make_partitions(N);
	uint64_t s = 0;
	for(size_t i = 0; i < primes_len; ++i){
		if(flags[primes[i]]){
			s += primes[i];
		}
	}
	free(flags);
	primesieve_free(primes);
	return s;
}

int main(){
	printf("S(q < N) = %"PRIu64"\n", sum_unipartition_primes(N));
}

