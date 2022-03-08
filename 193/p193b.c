#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <crater/hash.h>

uint64_t s(uint64_t n, cr8r_hashtbl_t *cache){
	uint64_t *v = cr8r_hash_get(cache, &cr8r_htft_u64_u64, &n);
	if(v){
		return v[1];
	}
	uint64_t res = n;
	for(uint64_t k = sqrt(n); k > 1; --k){
		res -= s(n/(k*k), cache);
	}
	//printf("s[%"PRIu64"]=%"PRIu64"\n", n, res);
	int status;
	uint64_t key[2] = {n, res};
	cr8r_hash_insert(cache, &cr8r_htft_u64_u64, key, &status);
	// TODO: consider checking status
	return res;
}

int main(){
	cr8r_hashtbl_t cache;
	if(!cr8r_hash_init(&cache, &cr8r_htft_u64_u64, 400000)){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate big cache!\e[0m\n");
		exit(1);
	}
	uint64_t res = s(1ull << 50, &cache);
	printf("s(%lli)=%"PRIu64"\n", 1ull << 50, res);
	printf("Cached %"PRIu64" results.\n", cache.full);
	cr8r_hash_destroy(&cache, &cr8r_htft_u64_u64);
}

