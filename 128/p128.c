/* The grid can be divided into 13 regions: the 1 at the center, the six spokes (2, 8, 20, ...; 3, 10, 23, ...; ...),
 * and the six triangular regions between them.  Looking at each of these regions except the top spoke and the 1, we
 * can see that each cell is bordered by the numbers 1 less than it and 1 greater than it, as well as 2 numbers of the
 * same parity (odd/even), with which it will have an even difference, except the top spoke and cells to its right.
 * This means unless the even difference is 2, the cell cannot have a PN of 3.  Since these differences (the even ones)
 * are between different rows, they increase and we can see the only one that is two is between 1 and 3.  Therefore,
 * the only cells which can have a PN of 3 are in the top spoke, to its right, or 1.  1 has a PN of 3.  For the others,
 * remember H(n) - H(n-1) = 6*n and look at this diagram:
 *           ________
 *  ________/H(n+1)+1\________
 * /H(n+1)+2\________/H(n+2)+0\
 * \________/H(n+0)+1\________/           ________
 * /H(n+0)+2\________/H(n+1)+0\  ________/H(n+1)+0\________
 * \________/H(n-1)+1\________/ /H(n-1)+1\________/H(n+1)-1\
 *          \________/          \________/H(n+0)+0\________/
 *                              /H(n-2)+1\________/H(n+0)-1\
 *                              \________/H(n-1)+0\________/
 *                                       \________/           
 * 
 *           ________
 *  ________/6*n+6   \________
 * /6*n+7   \________/12*n+17 \
 * \________/0       \________/           ________
 * /1       \________/6*n+5   \  ________/6*n+6   \________
 * \________/6*n     \________/ /6*n-1   \________/6*n+5   \
 *          \________/          \________/0       \________/
 *                              /12*n - 7\________/1       \
 *                              \________/6*n     \________/
 *                                       \________/
 * It's pretty clear which 3 differences can be prime for each type.
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define I 2000
#define DMR_PRIMES (uint64_t[]){2, 13, 23, 1662803}
#define DMR_PRIMES_C 4

uint64_t powmod(unsigned __int128 b, uint64_t e, uint64_t n){
	unsigned __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return (uint64_t)r;
}

int isPrime(uint64_t n){
	uint64_t s, d;//s, d | 2^s*d = n - 1
	if(n%2 == 0){
		return n == 2;
	}
	if(n == 1){
		return 0;
	}
	--n;
	s = __builtin_ctz(n);
	d = n>>s;
	++n;
	for(uint64_t i = 0, a, x; i < DMR_PRIMES_C; ++i){
		a = DMR_PRIMES[i];
		if(a >= n){
			break;
		}
		x = powmod(a, d, n);
		if(x == 1 || x == n - 1){
			goto CONTINUE_WITNESSLOOP;
		}
		for(a = 0; a < s - 1; ++a){
			x = powmod(x, 2, n);
			if(x == 1){
				return 0;
			}
			if(x == n - 1){
				goto CONTINUE_WITNESSLOOP;
			}
		}
		return 0;
		CONTINUE_WITNESSLOOP:;
	}
	return 1;
}

uint64_t H(uint64_t n){
	return 3*n*(n + 1) + 1;
}

int main(void){
	uint64_t i = 3, s = 0;
	for(uint64_t n = 2; n; ++n){
		if(!isPrime(6*n + 5)){
			continue;
		}
		if(isPrime(6*n - 1) && isPrime(12*n - 7)){
			++i;
			if(i == I){
				s = H(n);
				break;
			}
		}
		if(isPrime(12*n + 17) && isPrime(6*n + 7)){
			++i;
			if(i == I){
				s = H(n) + 1;
				break;
			}
		}
	}
	printf("%"PRIu64"\n", s);
}

