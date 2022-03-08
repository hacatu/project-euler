#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../util/hash.h"

const uint64_t MOD = 1000000007ull;

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
	.size= sizeof(uint64_t[2]),
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

uint64_t coprime_pairs_rec(uint64_t n, uint64_t dense_max, const uint64_t dense_counts[static dense_max + 1], hashtbl_t *sparse_counts){
	if(n <= dense_max){
		return dense_counts[n];
	}
	uint64_t *ent = hash_get(sparse_counts, &u64_counter_ft, &n);
	if(ent){
		return ent[1];
	}
	uint64_t res[2] = {n, n%MOD*n%MOD};
	uint64_t excl = 0;
	for(uint64_t m = 1; m <= dense_max; ++m){
		excl += dense_counts[m]*(n/m - n/(m+1));
	}
	excl %= MOD;
	for(uint64_t k = 2; k <= n/(dense_max + 1); ++k){
		excl += coprime_pairs_rec(n/k, dense_max, dense_counts, sparse_counts);
	}
	excl %= MOD;
	res[1] = (res[1] + MOD - excl)%MOD;
	int status;
	hash_insert(sparse_counts, &u64_counter_ft, res, &status);
	return res[1];
}

int coprime_pairs_init(uint64_t n, uint64_t *_dense_max, uint64_t **_dense_counts, hashtbl_t *sparse_counts){
	uint64_t dense_max = *_dense_max = sqrt(n);
	uint64_t *dense_counts = *_dense_counts = phi_sieve(dense_max + 1);
	if(!dense_counts){
		return 0;
	}
	for(uint64_t k = 2; k <= dense_max; ++k){
		dense_counts[k] = (2*dense_counts[k] + dense_counts[k - 1])%MOD;
	}
	//but now we need to deal with the counts c(n/k) where n/k is above the dense limit
	if(!hash_init(sparse_counts, &u64_counter_ft, dense_max*10)){
		return 0;
	}
	return 1;
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
	uint64_t res = 0;
	for(uint64_t t = 2; t <= N; t <<= 1){
		uint64_t part = coprime_pairs_rec(N/t, dense_max, dense_counts, &sparse_counts);
		part = (part - 1)*((MOD + 1)/2)%MOD;
		printf(" unordered pairs with gcd %"PRIu64": %"PRIu64"\n", t, part);
		res += part;
	}
	printf("Ascending 2-friendly pairs below %"PRIu64": %"PRIu64"\n", N, res%MOD);
	free(dense_counts);
	hash_destroy(&sparse_counts, &u64_counter_ft);
}

