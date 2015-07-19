#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000

#define calloca(size) memset(alloca(size), 0, size)

#define MAKE_WHEEL_AUTO(primes, ps...)                                 \
uint64_t wheelSize, *wheel, orthSize, *offsets, *offsets2, *skips;     \
({                                                                     \
	uint64_t p[] = {ps};                                               \
	uint64_t n = sizeof(p)/sizeof(uint64_t);                           \
	for(uint64_t i = 0; i < n; ++i){                                   \
		wheelSize *= p[i];                                             \
		HAC_VEC_PUSHR(uint64_t, &primes, p[i]);                        \
	}                                                                  \
	wheel = calloca(wheelSize*sizeof(uint64_t));                       \
	orthSize = preWheel(n, p, wheelSize, wheel);                       \
	offsets = alloca(orthSize*sizeof(uint64_t));                       \
	offsets2 = alloca(orthSize*sizeof(uint64_t));                      \
	skips = alloca(orthSize*sizeof(uint64_t));                         \
	makeWheel(wheelSize, wheel, orthSize, offsets, offsets2, skips);   \
})

#define MAKE_WHEEL_DYNAMIC(primes, ps...)                              \
uint64_t wheelSize, *wheel, orthSize, *offsets, *offsets2, *skips;     \
({                                                                     \
	uint64_t p[] = {ps};                                               \
	uint64_t n = sizeof(p)/sizeof(uint64_t);                           \
	for(uint64_t i = 0; i < n; ++i){                                   \
		wheelSize *= p[i];                                             \
		HAC_VEC_PUSHR(uint64_t, &primes, p[i]);                        \
	}                                                                  \
	wheel = calloc(wheelSize*sizeof(uint64_t));                        \
	orthSize = preWheel(n, p, wheelSize, wheel);                       \
	offsets = malloc(orthSize*sizeof(uint64_t));                       \
	offsets2 = malloc(orthSize*sizeof(uint64_t));                      \
	skips = malloc(orthSize*sizeof(uint64_t));                         \
	makeWheel(wheelSize, wheel, orthSize, offsets, offsets2, skips);   \
})

#define DELETE_WHEEL_DYNAMIC()                                         \
free(wheel);                                                           \
free(offsets);                                                         \
free(offsets2);                                                        \
free(skips);

HAC_VEC_T_NEW(uint64_t);

uint64_t preWheel(uint64_t n,
                  uint64_t *p,
                  uint64_t wheelSize,
                  uint64_t wheel[wheelSize]){
	uint64_t orthSize = wheelSize - n - 1;
	wheel[0] = 1;
	for(uint64_t i = 0; i < n; ++i){
		wheel[p[i]] = 1;
		for(uint64_t m = p[i]*p[i]; m < wheelSize; m += 2*p[i]){
			if(!wheel[m]){
				--orthSize;
			}
			wheel[m] = 1;
		}
	}
	return orthSize;
}

void makeWheel(uint64_t wheelSize,
               uint64_t wheel[wheelSize],
               uint64_t orthSize,
               uint64_t offsets[orthSize],
               uint64_t offsets2[orthSize],
               uint64_t skips[orthSize]){
	for(uint64_t i = 0, j = 0; i < wheelSize; ++i){
		if(!wheel[i]){
			offsets[j] = i;
			wheel[i] = j;
			++j;
		}else{
			wheel[i] = wheelSize;
		}
	}
	for(uint64_t j = 0, o; j < orthSize; ++j){
		o = offsets[j];
		o *= o;
		skips[j] = o/wheelSize;
		offsets2[j] = wheel[o%wheelSize];
	}
}

int main(void){
	clock_t start = clock();
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, MAX/2);
	MAKE_WHEEL_AUTO(primes, 2, 3);
	char isComposite[MAX/wheelSize*orthSize] = {0};
	for(uint64_t s = 5; s < MAX/wheelSize*orthSize; ++s){
		if(isComposite[n]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		for(uint64_t m = n*n; m < MAX; m += 2*n){
			isComposite[m] = 1;
		}
	}
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

