#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t *make_wheel(size_t *len, uint64_t *primes){
	size_t primes_len = *len;
	uint64_t wheel_len = 1;
	for(size_t i = 0; i < primes_len; ++i){
		wheel_len *= primes[i];
	}
	uint64_t *wheel = calloc(wheel_len, sizeof(uint64_t));
	if(!wheel){
		return NULL;
	}
	wheel[0] = 1;
	for(size_t i = 0; i < primes_len; ++i){
		for(uint64_t m = primes[i]; m < wheel_len; m += primes[i]){
			wheel[m] = 1;
		}
	}
	*len = wheel_len;
	return wheel;
}

/*
 * 0 1```5`
 **1 1```5`
 * 2 ```5`1
 * 3 `5`1``
 * 4 1```5`
 * 5 5`1```
 * 6 `5`1``
 * 7 `5`1``
 * 8 5`1```
 * 9 1```5`
 * A `5`1``
 **B ```5`1
 */

uint64_t *make_skiplist(size_t row, size_t *len, uint64_t *wheel){
	size_t wheel_len = *len, j = 0;
	uint64_t *skiplist = malloc(wheel_len*sizeof(uint64_t));
	int64_t _offsets[5], offsets = _offsets + 2;
	for(int64_t i = -2; i < 3; ++i){
		int64_t n = row + i;
		n = (n - 1)*n/2 + 1;
		n -= (row - 1)*row/2 + 1;
		offsets[i] = n%wheel_len;
	}
	if(!skiplist){
		return NULL;
	}
	switch(row%12){
	case 0:
		for(size_t i = 1; i < wheel_len; i += 2){
			if(!wheel[i]){
				if(!wheel[(i + offsets[-1] - 1)%wheel_len]){
					if((!wheel[(i - 2)%wheel_len]) || (!wheel[(i + offsets[+1])%wheel_len])){
						skiplist[j] = i - skiplist[j - 1];
						++j;
					}
				}else if((!wheel[(i + offsets[+1])%wheel_len]) && (!wheel[(i + offsets[+2] + 1)%wheel_len])){
					skiplist[j] = i - skiplist[j - 1];
					++j;
				}
			}
			i += 4;
			if(i >= wheel_len){
				break;
			}
			if(!wheel[i]){
				if(!wheel[(i + offsets[+1])%wheel_len]){
					if((!wheel[(i + offsets[+2] - 1)%wheel_len]) || (!wheel[(i + offsets[+2] + 1)%wheel_len]) || (!wheel[(i + offsets[-1] - 1)%wheel_len]) || (!wheel[(i + offsets[-1] + 1)%wheel_len])){
						skiplist[j] = i - skiplist[j - 1];
						++j;
					}
				}else if(!wheel[(i + offsets[-1] + 1)%wheel_len]){
					if((!wheel[(i + offsets[-1] - 1)%wheel_len]) || (!wheel[(i + offset[-2] - 1)%wheel_len])){
						skiplist[j] = i - skiplist[j - 1];
						++j;
					}
				}else if((!wheel[(i + offsets[-1] + 1)%wheel_len]) && (!wheel[(i + 2)%wheel_len])){
					skiplist[j] = i - skiplist[j - 1];
					++j;
				}
			}
		}
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10;
		break;
	case 11:
		break;
	}
}

int main(void){
	uint64_t primes[] = {2, 3, 5, 7, 11, 13, 17};
	size_t wheel_len = 5;
	uint64_t *wheel = make_wheel(&wheel_len, primes);
	uint64_t orth_c = 0;
	for(size_t i = 0; i < wheel_len; ++i){
		if(!wheel[i]){
			++orth_c;
		}
	}
	printf("%"PRIu64"/%zu\n", orth_c, wheel_len);
	free(wheel);
}

