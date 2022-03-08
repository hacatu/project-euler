#include <stdio.h>
#include <math.h>
#include <openssl/rand.h>

/* E(N) = 0*P(N=0) + 1*P(N=1) + 2*P(N=2) + ...
 * = 1*P(x_0!=-1 and x_1==-1) + ...
 * 
 * P(x_i-1!=-1 and x_i==-1) = P(x_i-1 has j<32 bits set and x_i has 32 bits set)
 * = P(x_i-1 has 0 bits set and x_i has 32 bits set) + ...
 * 
 * P(x_i-1 has j bits set and x_i has 32 bits set) = P(x_i-1 has j bits set)*2**(j - 32)
 * 
 * P(x_i-1 has j bits set) = P(the first 32 - j bits of x_i-1 are unset)*(32 choose j)
 * = 2**((j - 32)*(i - 2))*(32 choose j)
 * note this is wrong for i = 1 since x_0 is defined to be 0 so P(x_0 has j bits set) is
 * 1 if j is 0 and 0 otherwise
 * 
 * P(x_i-1 has j bits set and x_i has 32 bits set) = 2**((j - 32)*(i - 1))*(32 choose j)
 * = 2**((i - 1)*(-32))*(32 choose j)*(2**(i - 1))**j
 * 
 * P(x_i-1!=-1 and x_i==-1) = 2**((i - 1)*(-32))*((1 + 2**(i - 1))**32 - 2**((i - 1)*32))
 * except for i = 1 we have
 * P(x_0!=-1 and x_1==-1) = 2**-32
 * so we need to correct by 2**-32 - (2**32 - 1)
 * 
 * E(N) = 2**-32 - 2**32 + 1 + sum for i from 0 to infinity of i*(2**((i - 1)*(-32))*(1 + 2**(i - 1))**32 - 2**((i - 1)*32))
 */

int main(){
	uint64_t total_steps = 0;
	for(uint64_t i = 0; i < 100000; ++i){
		for(uint32_t x = 0, y; ~x;){
			++total_steps;
			RAND_bytes((void*)&y, sizeof(uint32_t));
			x |= y;
		}
	}
	printf("%f\n", total_steps/100000.);
}

