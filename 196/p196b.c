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

static inline uint64_t n_to_l(uint64_t n){
	uint64_t p = n + ((5 - n%6) & 3);//this adds r for n in n->a: 0->1,0->1,2->3,3->2,4->1,5->0 to make it be of the form 6k+-1
	return (p-4)/3;//sends 5->0, 7->1, ..., 6k-1 -> 2k-2, 6k+1 -> 2k-1
}

uint64_t S(uint64_t n){
	uint64_t a = (n-2)*(n-3)/2 + 1;
	uint64_t b = (n+3)*(n+2)/2;
	//[a, b] includes row n-2,n-1,n,n+1, and n+2 in their entirety
	//in particular there are ((n+2)*(n+3) - (n-3)*(n-2))/2 = (n*n + 5*n + 6 - n*n + 5*n - 6)/2 = 5*n
	//and when sieving we need to go up to the square root of (n+3)*(n+2)/2 "==" (n+3)/sqrt(2)
	uint64_t sieve_max = (n+4)*M_SQRT1_2;//(n+3)/sqrt(2) + fudge
	uint64_t *composite_mask = calloc((sieve_max + 64*3 - 1)/(64*3), sizeof(uint64_t));
	for(uint64_t k = 1; 6*(6*k*k - 2*k) + 1 < sieve_max; ++k){
		if(test_bit(composite_mask, 2*k - 2)){
			continue;
		}
		//printf("%"PRIu64" = 6*%"PRIu64"+1\n", 6*k - 1, k);
		//p = 6*k - 1
		//m = p*p = (6*k - 1)*(6*k - 1) = 6*(6*k*k - 2*k) + 1
		//m = p*(p+2) = (6*k - 1)*(6*k + 1) = 6*(6*k*k - 1) + 5
		//m = p*(p+6) = (6*k - 1)*(6*k + 5) = 6*(6*k*k - 2*k + 6*k - 1) + 1
		for(uint64_t l = 6*k*k - 2*k; 6*l + 1 < sieve_max; l += 6*k - 1){
			set_bit(composite_mask, 2*l - 1);
			if(6*(l + 2*k) - 1 >= sieve_max){
				break;
			}
			set_bit(composite_mask, 2*l + 4*k - 2);
		}
		if(6*(6*k*k + 2*k) + 1 >= sieve_max){
			break;
		}
		if(test_bit(composite_mask, 2*k - 1)){
			continue;
		}
		//p = 6*k + 1
		//m = p*p = (6*k + 1)*(6*k + 1) = 6*(6*k*k + 2*k) + 1
		//m = p*(p+4) = 6*(6*k*k + 6*k) + 5
		for(uint64_t l = 6*k*k + 2*k; 6*l + 1 < sieve_max; l += 6*k + 1){
			set_bit(composite_mask, 2*l - 1);
			if(6*(l + 4*k + 1) - 1 >= sieve_max){
				break;
			}
			set_bit(composite_mask, 2*l + 8*k);
		}
	}
	
	//now we sieve out on the interval [a, b]
	
	uint64_t *rows_mask = calloc((5*n + 64*3 - 1)/(64*3), sizeof(uint64_t));
	
	uint64_t l_a = n_to_l(a);
	
	for(uint64_t k = 1; 6*k - 1 < sieve_max; ++k){
		//a <= (6*k_q +- 1)(6*k_p +- 1)
		//ceil(a/(6*k_p +- 1)) <= 6*k_q +- 1
		//floor((a + p - 1)/p) <= 6*k_q +- 1
		if(!test_bit(composite_mask, 2*k - 2)){
			uint64_t p = 6*k - 1;
			uint64_t q = (a + p - 1)/p;
			uint64_t r = (5 - q%6) & 3;//this sends 0->1,0->1,2->3,3->2,4->1,5->0, so adding this to q will make it be of the form 6k+-1
			//maybe convert to using n_to_l or add another n_to_ function
			r = q + r;
			if(q%6 < 2){//6*r+1 is the first number to check in this case
				if((6*r + 1)*r > b){
					break;
				}
				//(6*q + 1)*(6*k - 1) = 36*q*k + 6*k - 6*q - 1 = 6*(q*k + k - q) - 1
				set_bit(rows_mask, 2*(r*k + k - r) - 2 - l_a);
				++r;
			}
		}
		if(6*k+5 >= sieve_max){
			break;
		}
		if(!test_bit(composite_mask, 2*k - 1)){
			printf("%"PRIu64"\n", 6*k+1);
		}
	}
	
	free(composite_mask);
	return 0;
}

int main(){
	S(300);
}

