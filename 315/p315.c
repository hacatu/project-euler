#define _POSIX_C_SOURCE 201805L
#include <inttypes.h>
#include <primesieve.h>
#include <stdio.h>
#include <time.h>

/*  <-0->
 * ^     ^
 * 1     2
 * v     v
 *  <-3->
 * ^     ^
 * 4     5
 * v     v
 *  <-6->
 */

uint64_t digit_masks[] = {
	[0]=0b1110111,
	[1]=0b0100100,
	[2]=0b1011101,
	[3]=0b1101101,
	[4]=0b0101110,
	[5]=0b1101011,
	[6]=0b1111011,
	[7]=0b0100111,
	[8]=0b1111111,
	[9]=0b1101111,
};

uint64_t sum_digits(uint64_t n){
	uint64_t s = 0;
	while(n){
		s += n%10;
		n /= 10;
	}
	return s;
}

uint64_t count_common_segments_single(uint64_t n, uint64_t m){
	uint64_t c = 0;
	while(m){
		c += __builtin_popcountll(digit_masks[n%10]&digit_masks[m%10]);
		n /= 10, m /= 10;
	}
	return c;
}

uint64_t count_common_segments(uint64_t n){
	uint64_t c = 0;
	while(n >= 10){
		uint64_t m = sum_digits(n);
		c += count_common_segments_single(n, m);
		n = m;
	}
	return c;
}

uint64_t calculate_switch_difference(){
	uint64_t tail_switch_differences[65] = {}, ret = 0;
	for(uint64_t n = 10; n < 65; ++n){
		tail_switch_differences[n] = count_common_segments(n);
	}
	size_t primes_size;
	uint64_t *primes = primesieve_generate_primes(10000000ull, 20000000ull, &primes_size, UINT64_PRIMES);
	for(uint64_t i = 0; i < primes_size; ++i){
		uint64_t p = primes[i];
		uint64_t n = sum_digits(p);
		ret += (count_common_segments_single(p, n) + tail_switch_differences[n]);
	}
	primesieve_free(primes);
	return ret<<1;
}

int main(){
	
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t res = calculate_switch_difference();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf("%"PRIu64"\ncalculated in %fs\n", res,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
	
	//printf("%"PRIu64"\n", count_common_segments(137));
}

