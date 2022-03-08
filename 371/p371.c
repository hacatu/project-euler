#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

inline uint32_t randint(uint32_t a, uint32_t b){
	uint32_t l = b - a, m = RAND_MAX/l*l, r;
	while((r = rand()) >= m);
	return r%l + a;
}

/* We have the numbers [0, 1, ..., 499, 500, 501, ..., 999].
 * 0 has a 1/1000 chance of being chosen, and can't be part of any
 * pair.  500 has a 1/1000 chance of being chosen and pairs with
 * itself.  The other numbers pair with each other.  Once there
 * are more than 499 unique numbers besides 0 and 500 there must be a win.
 * Define f(u, t) as the expected number of license plates until
 * a win where u is the number of unique numbers besides 0 but including 500
 * and t is whether or not 500 has been seen.  Then
 * f(u > 499, 0) = 0
 * f(u, 0) = (u + 1)/1000*(f(u, 0) + 1) + u/1000*1 + (998 - 2u)/1000*(f(u + 1, 0) + 1) + 1/1000*(f(u + 1, 1) + 1)
 * f(u > 500, 1) = 0
 * f(u, 1) = u/1000*(f(u, 1) + 1) + u/1000*1 + (1000 - 2u)/1000*(f(u + 1, 1) + 1)
 * Move out the +1 in all cases and remove the common denominator of the probabilities of 1000
 * f(u, 0) = ((u + 1)f(u, 0) + (998 - 2u)f(u + 1, 0) + f(u + 1, 1) + 1000)/1000
 * f(u, 1) = (u*f(u, 1) + (1000 - 2u)f(u + 1, 1) + 1000)/1000
 * Now solve for f(u, t) in terms of f(u + 1, t)
 * f(u, 0) = ((998 - 2u)f(u + 1, 0) + f(u + 1, 1) + 1000)/(999 - u)
 * f(u, 1) = ((1000 - 2u)f(u + 1, 1) + 1000)/(1000 - u)
 * We want to know f(0, 0).  By inspecting this recurrence relation,
 * we can see that f(500, 1) does not depend on f(501, 1) since it has a zero coefficient.
 * So we can set f(501, 1) = 0.  f(499, 0) does not depend on f(500, 0) for the same reason,
 * but it does depend on f(500, 1), so we need to calculate f(500, 1) first.
 * From our equation, f(500, 1) = ((1000 - 2*500)f(501, 1) + 1000)/(1000 - 500) = 2
 * So we have f(500, 0) = 0 and f(500, 1) = 2.  Now we can just iterate the recurrence relation to get f(0, 0).
 */

double e_calc(void){
	double a = 0, b = 2, u = 500;
	while(--u){
		a = ((998 - 2*u)*a + b + 1000)/(999 - u);
		b = ((1000 - 2*u)*b + 1000)/(1000 - u);
	}
	return (998*a + b + 1000)/999;
}

double e_calc_exact(void){
	mpq_t a, b, t, m;
	uint64_t u = 500;
	mpq_init(a);
	mpq_init(b);
	mpq_init(t);
	mpq_init(m);
	mpq_set_ui(a, 0, 1);
	mpq_set_ui(b, 2, 1);
	mpq_set_ui(m, 1000, 1);
	while(--u){
		mpq_set_ui(t, 998 - (u << 1), 1);
		mpq_mul(a, t, a);
		mpq_add(a, a, b);
		mpq_add(a, a, m);
		mpq_set_ui(t, 999 - u, 1);
		mpq_div(a, a, t);
		mpq_set_ui(t, 1000 - (u << 1), 1);
		mpq_mul(b, t, b);
		mpq_add(b, b, m);
		mpq_set_ui(t, 1000 - u, 1);
		mpq_div(b, b, t);
	}
	mpq_set_ui(t, 998, 1);
	mpq_mul(a, t, a);
	mpq_add(a, a, b);
	mpq_add(a, a, m);
	mpq_set_ui(t, 999, 1);
	mpq_div(a, a, t);
	double r = mpq_get_d(a);
	mpq_clear(m);
	mpq_clear(t);
	mpq_clear(b);
	mpq_clear(a);
	return r;
}

double e_test(uint64_t t){
	uint64_t s = 0;
	for(uint64_t i = 0; i < t; ++i){
		uint64_t cmpl[16] = {};
		for(uint64_t j = 1;; ++j){
			uint32_t n = randint(0, 1000);
			if(cmpl[(1000 - n) >> 6] & (1ULL << (n&0x3F))){
				s += j;
				break;
			}
			cmpl[n >> 6] |= 1ULL << (n&0x3F);
		}
	}
	return (double)s/t;
}

int main(void){
	double r, o;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	o = (end.tv_sec - start.tv_sec) + 1e-9*(end.tv_nsec - start.tv_nsec);
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	r = e_calc();
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	printf("Calculated: %.8f in %.9fs\n", r, (end.tv_sec - start.tv_sec) + 1e-9*(end.tv_nsec - start.tv_nsec) - o);
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	r = e_calc_exact();
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	printf("Rational: %.8f in %.9fs\n", r, (end.tv_sec - start.tv_sec) + 1e-9*(end.tv_nsec - start.tv_nsec) - o);
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	r = e_test(1000000);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	printf("Experemental: %.8f in %.9fs\n", r, (end.tv_sec - start.tv_sec) + 1e-9*(end.tv_nsec - start.tv_nsec) - o);
}

