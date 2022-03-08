#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>

#define MIN 2
#define MAX 45

mpz_t iss[MAX - MIN + 1];
mpz_t rss[MAX - MIN + 1];
mpz_t tmp[MAX - MIN + 1];
bool sctd[MAX - MIN + 1] = {0};
mpz_t half;

void __attribute__((constructor)) ctor(void){
	mpz_t denom;
	mpz_init(denom);
	mpz_set_ui(denom, 1);
	for(unsigned long i = MIN; i <= MAX; ++i){
		mpz_lcm_ui(denom, denom, i*i);
	}
	mpz_init(half);
	mpz_divexact_ui(half, denom, 2);
	for(unsigned long i = MIN; i <= MAX; ++i){
		mpz_init(iss[i - MIN]);
		mpz_divexact_ui(iss[i - MIN], denom, i*i);
		mpz_init(rss[i - MIN]);
		mpz_init(tmp[i - MIN]);
	}
	mpz_clear(denom);
	mpz_set(rss[MAX - MIN], iss[MAX - MIN]);
	for(unsigned long i = MAX - 1; i >= MIN; --i){
		mpz_add(rss[i - MIN], iss[i - MIN], rss[i + 1 - MIN]);
	}
	//mpz_sub(half, rss[0], half);
}

void __attribute__((destructor)) dtor(void){
	mpz_clear(half);
	for(unsigned long i = MIN; i <= MAX; ++i){
		mpz_clear(iss[i - MIN]);
		mpz_clear(rss[i - MIN]);
		mpz_clear(tmp[i - MIN]);
	}
}

bool findFirstGreedy(const mpz_t s, const mpz_t *iss, const mpz_t *rss, bool *sctd, size_t l){
	if(!mpz_cmp_ui(s, 0)){
		return true;
	}
	for(size_t i = 0; i < l; ++i){
		if(mpz_cmp(s, iss[i]) >= 0){
			if(mpz_cmp(s, rss[i]) > 0){
				return false;
			}
			mpz_sub(tmp[l - 1], s, iss[i]);
			sctd[i] = true;
			if(findFirstGreedy(tmp[l - 1], iss + i + 1, rss + i + 1, sctd + i + 1, l - i - 1)){
				return true;
			}
			sctd[i] = false;
		}
	}
	return false;
}

int main(void){
	if(!findFirstGreedy(half, iss, rss, sctd, MAX - MIN + 1)){
		puts("Could not make half");
		return 1;
	}
	for(size_t i = MIN; i <= MAX; ++i){
		if(sctd[i - MIN]){
			printf("%lu,", i);
		}
	}
}

