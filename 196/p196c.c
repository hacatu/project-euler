#define _GNU_SOURCE

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static inline uint64_t test_bit(const uint64_t *bit_mask, uint64_t i){
	return bit_mask[i >> 6] & (1ull << (i & 0x3F));
}

static inline void set_bit(uint64_t *bit_mask, uint64_t i){
	bit_mask[i >> 6] |= (1ull << (i & 0x3F));
}

static inline int is_triplet_prime(uint64_t i, uint64_t n, uint64_t a, uint64_t h, const uint64_t *rows_mask){
	if(test_bit(rows_mask, h + i - a)){
		return 0;
	}
	if(n&1){
		if(!test_bit(rows_mask, h + i - n + 1 - a)){//cell above
			return !test_bit(rows_mask, h + i - 2*n + 2 - a) ||//cell 2 above and 1 left
			       !test_bit(rows_mask, h + i - 2*n + 4 - a) ||//cell 2 above and 1 right
			       !test_bit(rows_mask, h + i + n - 1 - a) ||//cell 1 below and 1 left
			       !test_bit(rows_mask, h + i + n + 1 - a);//cell 1 below and 1 right
		}else if(!test_bit(rows_mask, h + i + n - 1 - a)){//cell 1 below and 1 left
			return !test_bit(rows_mask, h + i - 2 - a) ||//cell 2 to the left
			       !test_bit(rows_mask, h + i + 2*n - a) ||//cell 2 below and 1 left
			       !test_bit(rows_mask, h + i + n + 1 - a);//cell 1 below and 1 right
		}else if(!test_bit(rows_mask, h + i + n + 1 - a)){//cell 1 below and 1 right
			return !test_bit(rows_mask, h + i + 2*n + 2 - a) ||//cell 2 below and 1 right
			       !test_bit(rows_mask, h + i + 2 - a);//cell 2 to the right
		}
	}else{
		if(!test_bit(rows_mask, h + i + n - a)){//cell below
			return !test_bit(rows_mask, h + i + 2*n - a) ||//cell 2 below and 1 left
			       !test_bit(rows_mask, h + i + 2*n + 2 - a) ||//cell 2 below and 1 right
			       !test_bit(rows_mask, h + i - n - a) ||//cell 1 above and 1 left
			       !test_bit(rows_mask, h + i - n + 2 - a);//cell 1 above and 1 right
		}else if(!test_bit(rows_mask, h + i - n - a)){//cell 1 above and 1 left
			return !test_bit(rows_mask, h + i - 2 - a) ||//cell 2 to the left
			       !test_bit(rows_mask, h + i - 2*n + 2 - a) ||//cell 2 above and 1 left
			       !test_bit(rows_mask, h + i - n + 2 - a);//cell 1 above and 1 right
		}else if(!test_bit(rows_mask, h + i - n + 2 - a)){//cell 1 above and 1 right
			return !test_bit(rows_mask, h + i - 2*n + 4 - a) ||//cell 2 above and 1 right
			       !test_bit(rows_mask, h + i + 2 - a);//cell 2 to the right
		}
	}
	return 0;
}

static inline int is_triplet_prime_edge(uint64_t i, uint64_t n, uint64_t a, uint64_t h, const uint64_t *rows_mask){
	if(test_bit(rows_mask, h + i - a)){
		return 0;
	}
	if(n&1){
		if(!test_bit(rows_mask, h + i - n + 1 - a)){//cell above
			return (i > 0     && !test_bit(rows_mask, h + i - 2*n + 2 - a)) ||//cell 2 above and 1 left
			       (i + 3 < n && !test_bit(rows_mask, h + i - 2*n + 4 - a)) ||//cell 2 above and 1 right
			       (i > 0     && !test_bit(rows_mask, h + i + n - 1 - a)) ||//cell 1 below and 1 left
			                     !test_bit(rows_mask, h + i + n + 1 - a);//cell 1 below and 1 right
		}else if(i > 0 && !test_bit(rows_mask, h + i + n - 1 - a)){//cell 1 below and 1 left
			return (i > 1     && !test_bit(rows_mask, h + i - 2 - a)) ||//cell 2 to the left
			       (i > 0     && !test_bit(rows_mask, h + i + 2*n - a)) ||//cell 2 below and 1 left
			                     !test_bit(rows_mask, h + i + n + 1 - a);//cell 1 below and 1 right
		}else if(!test_bit(rows_mask, h + i + n + 1 - a)){//cell 1 below and 1 right
			return               !test_bit(rows_mask, h + i + 2*n + 2 - a) ||//cell 2 below and 1 right
			       (i + 2 < n && !test_bit(rows_mask, h + i + 2 - a));//cell 2 to the right
		}
	}else{
		if(!test_bit(rows_mask, h + i + n - a)){//cell below
			return (i > 0     && !test_bit(rows_mask, h + i + 2*n - a)) ||//cell 2 below and 1 left
			                     !test_bit(rows_mask, h + i + 2*n + 2 - a) ||//cell 2 below and 1 right
			       (i > 0     && !test_bit(rows_mask, h + i - n - a)) ||//cell 1 above and 1 left
			       (i + 2 < n && !test_bit(rows_mask, h + i - n + 2 - a));//cell 1 above and 1 right
		}else if(i > 0 && !test_bit(rows_mask, h + i - n - a)){//cell 1 above and 1 left
			return (i > 1     && !test_bit(rows_mask, h + i - 2 - a)) ||//cell 2 to the left
			       (i > 0     && !test_bit(rows_mask, h + i - 2*n + 2 - a)) ||//cell 2 above and 1 left
			       (i + 2 < n && !test_bit(rows_mask, h + i - n + 2 - a));//cell 1 above and 1 right
		}else if(i + 2 < n && !test_bit(rows_mask, h + i - n + 2 - a)){//cell 1 above and 1 right
			return (i + 3 < n && !test_bit(rows_mask, h + i - 2*n + 4 - a)) ||//cell 2 above and 1 right
			       (i + 2 < n && !test_bit(rows_mask, h + i + 2 - a));//cell 2 to the right
		}
	}
	return 0;
}

uint64_t S(uint64_t n){
	uint64_t a = (n-2)*(n-3)/2 + 1;
	uint64_t b = (n+3)*(n+2)/2;
	uint64_t h = n*(n-1)/2 + 1;
	//[a, b] includes row n-2,n-1,n,n+1, and n+2 in their entirety
	//in particular there are ((n+2)*(n+3) - (n-3)*(n-2))/2 = (n*n + 5*n + 6 - n*n + 5*n - 6)/2 = 5*n
	//and when sieving we need to go up to the square root of (n+3)*(n+2)/2 "==" (n+3)/sqrt(2)
	uint64_t sieve_max = (n+4)*M_SQRT1_2;//(n+3)/sqrt(2) + fudge
	uint64_t *composite_mask = calloc((sieve_max + 64 - 1)/(64), sizeof(uint64_t));
	printf("Finding \"prime triplets\" in row %"PRIu64": creating list of sieving primes up to %"PRIu64"\n", n, sieve_max);
	for(uint64_t k = 2; k < sieve_max; ++k){
		if(test_bit(composite_mask, k)){
			continue;
		}
		//printf(" sieving prime %"PRIu64"\n", k);
		for(uint64_t m = k*k; m < sieve_max; m += k){
			set_bit(composite_mask, m);
		}
	}
	
	//now we sieve out on the interval [a, b]
	
	uint64_t *rows_mask = calloc((5*n + 64 - 1)/(64), sizeof(uint64_t));
	printf("Sieving interval [%"PRIu64", %"PRIu64"] with sieving primes\n", a, b);
	
	for(uint64_t k = 2; k < sieve_max; ++k){
		if(!test_bit(composite_mask, k)){
			//printf(" sieving prime %"PRIu64"\n", k);
			for(uint64_t m = a + k - 1 - (a - 1)%k; m <= b; m += k){
				set_bit(rows_mask, m - a);
			}
		}
	}
	
	free(composite_mask);
	uint64_t res = 0;
	for(uint64_t i = 0; i < 2; ++i){
		if(is_triplet_prime_edge(i, n, a, h, rows_mask)){
			res += h + i;
		}
	}
	for(uint64_t i = 2; i + 3 < n; ++i){
		if(is_triplet_prime(i, n, a, h, rows_mask)){
			res += h + i;
		}
	}
	for(uint64_t i = n - 3; i < n; ++i){
		if(is_triplet_prime_edge(i, n, a, h, rows_mask)){
			res += h + i;
		}
	}
	free(rows_mask);
	return res;
}

int main(){
	printf("%"PRIu64"\n", S(5678027) + S(7208785));
}

