#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000

HAC_VEC_T_NEW(uint64_t);
//a mod 30 wheel rotated twice
const uint8_t offsets[16] = {6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2};

const uint8_t lut_offsets[] = {
	
};

const uint8_t first_q_moduli[60] = {[1]=1,[13]=1,[17]=1,[29]=1,[37]=1,[41]=1,[49]=1,[53]=1};
const uint8_t second_q_moduli[60] = {[7]=1,[19]=1,[31]=1,[43]=1};
const uint8_t third_q_moduli[60] = {[11]=1,[23]=1,[47]=1,[59]=1};

void firstQuadratic(char is_prime[MAX]){
	//4*x^2 + y^2;
	for(uint64_t cx2 = 4, dcx2 = 12; cx2 + 1 < MAX; cx2 += dcx2, dcx2 += 8){
		for(uint64_t cx2py2 = cx2 + 1, dy2 = 3; cx2py2 < MAX; cx2py2 += dy2, dy2 += 2){
			if(cx2py2 < 7){
				continue;
			}
			if(!first_q_moduli[cx2py2%60]){
				continue;
			}
			is_prime[cx2py2] ^= 1;
		}
	}
}

void secondQuadratic(char is_prime[MAX]){
	//3*x^2 + y^2;
	for(uint64_t cx2 = 3, dcx2 = 9; cx2 + 1 < MAX; cx2 += dcx2, dcx2 += 6){
		for(uint64_t cx2py2 = cx2 + 1, dy2 = 3; cx2py2 < MAX; cx2py2 += dy2, dy2 += 2){
			if(cx2py2 < 7){
				continue;
			}
			if(!second_q_moduli[cx2py2%60]){
				continue;
			}
			is_prime[cx2py2] ^= 1;
		}
	}
}

void thirdQuadratic(char is_prime[MAX]){
	//3*x^2 - y^2;
	for(uint64_t cx2 = 3, dcx2 = 9; cx2/3*2 < MAX; cx2 += dcx2, dcx2 += 6){
		for(uint64_t cx2my2 = cx2/3*2, dy2 = dcx2/3 - 2; cx2my2 < MAX && cx2my2 < cx2; cx2my2 += dy2, dy2 -= 2){
			if(cx2my2 < 7){
				continue;
			}
			if(!third_q_moduli[cx2my2%60]){
				continue;
			}
			is_prime[cx2my2] ^= 1;
		}
	}
}

int main(void){
	clock_t start = clock();
	char is_prime[MAX] = {[2]=1,[3]=1,[5]=1};
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, MAX/2);
	HAC_VEC_PUSHR(uint64_t, &primes, 2);
	HAC_VEC_PUSHR(uint64_t, &primes, 3);
	HAC_VEC_PUSHR(uint64_t, &primes, 5);
	firstQuadratic(is_prime);
	secondQuadratic(is_prime);
	thirdQuadratic(is_prime);
	for(uint64_t n = 7, s1 = 1; n < MAX; n += offsets[s1], ++s1 == 8 ? s1 = 0 : 0){
		if(!is_prime[n]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		for(uint64_t m = n*n; m < MAX; m += n*n){
			is_prime[m] = 0;
		}
	}
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

