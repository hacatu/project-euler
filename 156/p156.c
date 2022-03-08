#include <stdio.h>
#include <inttypes.h>

static uint64_t f(uint64_t d, uint64_t n);
static uint64_t s(uint64_t d);
static uint64_t ebsearch(uint64_t d, uint64_t n, uint64_t fn);
static uint64_t ebsearch_rec(uint64_t d, uint64_t n, uint64_t a, uint64_t fa, uint64_t b, uint64_t fb);

/*
 * f(n, d) = the number of times the digit d appears in all numbers on [0, n].
 * f(n, d) is always at least f(<n except the first digit>, d) + <first digit of n>*<length of n, -1>*10^<length of n, -2>.
 * When the first digit of n = d, add <n except the first digit, +1>.
 * When the first digit of n > d, add 10^<length of n, -1>.
 * Note that these formulas sometimes call for 10^-1, which should be 0 (because of truncation)
 * and n%0 should be 0.  Since that is not how math works the code has a special case.
 * To get an idea of the upper bound for numbers n such that f(n, d) = n, consider that there
 * is a 1/10 chance of any digit being d so the expected number of ds in n is <length of n>/10.
 * So, roughly speaking, for numbers with more than 10 digits, more than one d is contributed
 * by each number so the number of ds rises faster than n.
 * 
 * NOTE: the parameters are reversed for consistency with the other functions.
 */

uint64_t f(uint64_t d, uint64_t n){
	uint64_t r = 0, m = n;
	//l = length - 1, t = 10^(length - 1)
	for(uint64_t l = 0, t = 1; m; m /= 10, ++l, t *= 10){
		if(l){
			r += m%10*l*(t/10);
			if(m%10 == d){
				r += n%t + 1;
			}else if(m%10 > d){
				r += t;
			}
		}else if(m%10 >= d){
			++r;
		}
	}
	return r;
}

uint64_t s(uint64_t d){
	uint64_t ret = 0;
	for(uint64_t n = 0; n < 10000000000UL*d;){
		uint64_t fn = f(n, d);
		if(fn == n){
			ret += n++;
		}else if(fn > n){
			n = fn;
		}else{
			
		}
	}
}

uint64_t ebsearch(uint64_t d, uint64_t n, uint64_t fn){
	return ebsearch_rec(d, n, n, fn, );
}

int main(void){
	printf("%"PRIu64"\n", f(20000000000UL, 2));
}

