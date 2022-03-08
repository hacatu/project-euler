#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../util/hash.h"

const unsigned __int128 MOD = 1000000007ull;

typedef struct{
	uint64_t n;
	unsigned __int128 count;
} counter_ent;

inline static uint64_t u64_hash(const void *a){
	return *(const uint64_t*)a;
}

inline static int u64_cmp(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}
	return 0;
}

hashtbl_ft u64_counter_ft = {
	.size= sizeof(counter_ent),
	.hash= u64_hash,
	.cmp= u64_cmp,
	.load_factor= 0.7,
};

uint64_t *phi_sieve(uint64_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint64_t *phi = malloc(max*sizeof(uint64_t));
	if(!phi){
		return NULL;
	}
	for(uint64_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint64_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] != p){
			continue;
		}
		for(uint64_t n = p; n < max; n += p){
			phi[n] /= p;
			phi[n] *= p - 1;
		}
		//printf("phi(%" PRIu32 ") = %" PRIu32 "\n", p, phi[p]);
	}
	return phi;
}

unsigned __int128 coprime_pairs_rec(uint64_t n, uint64_t dense_max, const uint64_t dense_counts[static dense_max + 1], hashtbl_t *sparse_counts){
	if(n <= dense_max){
		return dense_counts[n];
	}
	counter_ent *ent = hash_get(sparse_counts, &u64_counter_ft, &n);
	if(ent){
		return ent->count;
	}
	counter_ent res = {n, (unsigned __int128)n*n};
	for(uint64_t m = 1; m <= dense_max; ++m){
		res.count -= (unsigned __int128)dense_counts[m]*(n/m - n/(m+1));
	}
	for(uint64_t k = 2; k <= n/(dense_max + 1); ++k){
		res.count -= coprime_pairs_rec(n/k, dense_max, dense_counts, sparse_counts);
	}
	int status;
	hash_insert(sparse_counts, &u64_counter_ft, &res, &status);
	return res.count;
}

int coprime_pairs_init(uint64_t n, uint64_t *_dense_max, uint64_t **_dense_counts, hashtbl_t *sparse_counts){
	uint64_t dense_max = *_dense_max = sqrt(n);
	uint64_t *dense_counts = *_dense_counts = phi_sieve(dense_max + 1);
	if(!dense_counts){
		return 0;
	}
	for(uint64_t k = 2; k <= dense_max; ++k){
		dense_counts[k] = 2*dense_counts[k] + dense_counts[k - 1];
	}
	//but now we need to deal with the counts c(n/k) where n/k is above the dense limit
	if(!hash_init(sparse_counts, &u64_counter_ft, dense_max*10)){
		return 0;
	}
	return 1;
}

int print_u128(unsigned __int128 n){
	char _buf[40];
	char *buf = _buf + 39;
	*buf = '\0';
	if(!n){
		return printf("0");
	}
	while(n){
		*--buf = '0' + n%10;
		n /= 10;
	}
	return printf(buf);
}

int main(){
	uint64_t N = 100000000000ull;
	uint64_t dense_max;
	uint64_t *dense_counts;
	hashtbl_t sparse_counts;
	if(!coprime_pairs_init(N/2, &dense_max, &dense_counts, &sparse_counts)){
		printf("Couldn't initialize structures for counting coprime pairs (probably no space)\n");
		exit(EXIT_FAILURE);
	}
	printf("Begin counting coprime pairs (this will probably take 12 minutes)\n");
	unsigned __int128 res = 0;
	for(uint64_t t = 2; t <= N; t <<= 1){
		unsigned __int128 part = coprime_pairs_rec(N/t, dense_max, dense_counts, &sparse_counts);
		part = (part - 1)/2;
		printf(" unordered pairs with gcd %"PRIu64": ", t);
		print_u128(part);
		putc('\n', stdout);
		res += part;
	}
	printf("Ascending 2-friendly pairs below %"PRIu64": ", N);
	print_u128(res);
	putc('\n', stdout);
	free(dense_counts);
	hash_destroy(&sparse_counts, &u64_counter_ft);
}

