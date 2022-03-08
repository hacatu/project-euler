#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/hash.h"

/*
 * f(0) = 0, f(1) = 1,
 * f(2n) = f(n)
 * f(4n + 1) = 2f(2n + 1) - f(n)
 * f(4n + 3) = 3f(2n + 1) - 2f(n)
 * 
 * S(n) = sum(i=0,n, f(i))
 * S(n) = sum(i=0,n,i=0%2, f(i/2)) + sum(i=0,n,i=1%4, 2f((i+1)/2) - f((i-1)/4)) + sum(i=0,n,i=3%4, 3f((i-1)/2) - 2f((i-3)/4))
 * S(n) = sum(k=0,floor(n/2), f(k)) + sum(k=0,floor((n-1)/4), 2f(2k+1) - f(k)) + sum(k=0,floor((n-3)/4), 3f(2k+1) - 2f(k))
 * 
 * Let
 * T(n) = sum(i=0,n, f(2i+1))
 * T(n) = sum(i=0,n,i=0%2, 2f(i+1) - f(i/2)) + sum(i=0,n,i=1%2, 3f(i) - f((i-1)/2))
 * T(n) = sum(k=0,floor(n/2), 2f(2k+1) - f(k)) + sum(k=0,floor((n-1)/2), 3f(2k+1) - 2f(k))
 * 
 * S(n) = S(floor(n/2)) + 2T(floor((n-1)/4)) - S(floor((n-1)/4)) + 3T(floor((n-3)/4)) - 2S(floor((n-3)/4))
 * T(n) = 2T(floor(n/2)) - S(floor(n/2)) + 3T(floor((n-1)/2)) - 2S(floor((n-1)/2))
 */

/*
uint64_t T(uint64_t n);
uint64_t S(uint64_t n){
	const static uint64_t small_s[] = {0, 1, 2, 5};
	if(n < 4){
		return small_s[n];
	}
	return S(n/2) + 2*T((n-1)/4) - S((n-1)/4) + 3*T((n-3)/4) - 2*S((n-3)/4) - 1;
}

uint64_t T(uint64_t n){
	const static uint64_t small_t[] = {1, 4, 8, 15};
	if(n < 4){
		return small_t[n];
	}
	return 2*T(n/2) - S(n/2) + 3*T((n-1)/2) - 2*S((n-1)/2) - 1;
}
*/

inline static uint64_t u64_hash(const void *a){
	return *(const uint64_t*)a;
}

inline static int u64_cmp(const void *_a, const void *_b){
	int64_t a = *(const int64_t*)_a, b = *(const int64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}
	return 0;
}

hashtbl_ft u64_counter_ft = {
	.size= sizeof(int64_t[2]),
	.hash= u64_hash,
	.cmp= u64_cmp,
	.load_factor= 0.7,
};

int64_t *f_table;
int64_t *S_table;
hashtbl_t S_memo;
const int64_t table_len = 387420489;//3**18
const int64_t MOD = 1000000000;

int64_t f(int64_t n){
	n >>= __builtin_ctzll(n);
	if(n < 2){
		return n;
	}
	if(n&2){
		return 3*f_table[n >> 1] - 2*f_table[n >> 2];
	}
	return 2*f_table[(n >> 1) + 1] - f_table[n >> 2];
}

int64_t S(int64_t n){
	if(n < table_len){
		return S_table[n];
	}
	int64_t *ent = hash_get(&S_memo, &u64_counter_ft, &n);
	if(ent){
		return ent[1];
	}
	int64_t res[2] = {n, 0};
	switch(n&3){
	case 0:
		res[1] = S(n/2) + 5*S(n/2 - 1) - 8*S(n/4 - 1) - 1;
	case 1:
		res[1] = S(n/2) + 2*S(n/2 + 1) - 3*S(n/4) + 3*S(n/2 - 1) - 5*S(n/4 - 1) - 1;
	case 2:
		res[1] = 3*S(n/2) - 3*S(n/4) + 3*S(n/2 - 2) - 5*S(n/4 - 1) - 1;
	case 3:
		res[1] = 6*S(n/2) - 8*S(n/4) - 1;
	}
	//return S(n/2) + 2*S((n-1)/4*2 + 1) - 3*S((n-1)/4) + 3*S((n-3)/4*2 + 1) - 5*S((n-3)/4) - 1;
	res[1] %= MOD;
	if(res[1] < 0){
		res[1] += MOD;
	}
	int status;
	hash_insert(&S_memo, &u64_counter_ft, res, &status);
	return res[1];
}

int main(){
	printf("Populating tables\n");
	f_table = malloc(table_len*sizeof(int64_t));
	S_table = malloc(table_len*sizeof(int64_t));
	for(int64_t n = 0; n < table_len; ++n){
		f_table[n] = f(n);
	}
	S_table[0] = 0;
	for(int64_t n = 1; n < table_len; ++n){
		S_table[n] = (S_table[n - 1] + f_table[n])%MOD;
	}
	printf("Done\n");
	free(f_table);
	hash_init(&S_memo, &u64_counter_ft, 531441);
	for(int64_t N = 3, p = 1; p <= 37; N *= 3, ++p){
		printf("S(3^%"PRId64")=%"PRId64"\n", p, S(N));
	}
	hash_destroy(&S_memo, &u64_counter_ft);
	free(S_table);
}

