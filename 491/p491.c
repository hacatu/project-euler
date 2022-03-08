#define _POSIX_C_SOURCE 201805L
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int verify_sub(uint64_t caps_len, const uint64_t caps[static caps_len], const uint64_t sub[static caps_len], uint64_t total, uint64_t sum){
	uint64_t c = 0, s = 0;
	for(uint64_t i = 0; i < caps_len; ++i){
		if(sub[i] > caps[i]){
			return 0;
		}
		c += sub[i];
		s += sub[i]*i;
	}
	return c == total && s == sum;
}

uint64_t count_permutations(uint64_t caps_len, const uint64_t sub[static caps_len]){
	uint64_t ret = (10 - *sub)*1316818944000ull;//9!*10!
	for(uint64_t i = 0; i < caps_len; ++i){
		if(sub[i] == 2){
			ret >>= 2;//this has the effect of dividing by four to the power of the number of doubles
		}
	}
	return ret;
}

uint64_t fill_leading_submultiset(uint64_t caps_len, const uint64_t caps[static caps_len], uint64_t sub[static caps_len], uint64_t total){
	uint64_t sub_sum = 0;
	for(uint64_t i = 0; i < caps_len; ++i){
		if(caps[i] < total){
			total -= sub[i] = caps[i];
			sub_sum += caps[i]*i;
		}else{
			sub[i] = total;
			sub_sum += total*i;
			total = 0;
			break;
		}
	}
	return total ? UINT64_MAX : sub_sum;
}

uint64_t generate_submultisets(uint64_t caps_len, const uint64_t caps[static caps_len], uint64_t total){
	uint64_t *sub, *sub_n, _buf[2*caps_len], ret = 0;
	sub = _buf;
	sub_n = _buf + caps_len;
	memset(sub, 0, caps_len*sizeof(uint64_t));
	uint64_t sub_sum = fill_leading_submultiset(caps_len, caps, sub, total);
	if(sub_sum == UINT64_MAX){
		return UINT64_MAX;//the total argument was larger than the cardinality of a multiset with multiplicities described by caps
	}
	/*uint64_t sub_sum_counts[70 - 20 + 1];
	memset(sub_sum_counts, 0, (70 - 20 + 1)*sizeof(uint64_t));*/
	while(1){
		//process
		/*++sub_sum_counts[sub_sum - 20];*/
		if(sub_sum%11 == 1){
			ret += count_permutations(caps_len, sub);
		}
		//increment
		uint64_t i, j, head_total = 0, head_sum = 0;
		for(i = 0; i < caps_len && !sub[i]; ++i);
		if(i == caps_len){
			break;//the total argument was 0
		}
		head_total = sub[i] - 1;
		head_sum = head_total*i;
		for(j = i + 1; j < caps_len && sub[j] == caps[j]; ++j){
			head_total += sub[j];
			head_sum += sub[j]*j;
		}
		if(j == caps_len){
			break;//all submultisets of cardinality equal to the total argument have been processed
		}
		memcpy(sub_n + j, sub + j, (caps_len - j)*sizeof(uint64_t));
		memset(sub_n, 0, j*sizeof(uint64_t));
		uint64_t head_sum_n = fill_leading_submultiset(caps_len, caps, sub_n, head_total);
		sub = sub_n;
		sub_n = caps_len - (sub - _buf) + _buf;
		++sub[j];
		sub_sum = sub_sum + j + head_sum_n - head_sum - i;
	}
	/*for(uint64_t n = 20; n <= 70; ++n){
		printf("%3"PRIu64" subs with sum %"PRIu64"\n", sub_sum_counts[n - 20], n);
	}*/
	return ret;
}

int main(){
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t res = generate_submultisets(10, (uint64_t[10]){2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 10);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf("%"PRIu64"\n%fs\n", res,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
}

