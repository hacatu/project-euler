#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>

#define MIN 2
#define MAX 45

mpq_t iss[MAX - MIN + 1];
mpq_t rss[MAX - MIN + 1];
mpq_t zero;
mpq_t half;
bool sctd[MAX - MIN + 1] = {0};

void __attribute__((constructor)) ctor(void){
	mpq_init(zero);
	mpq_init(half);
	mpq_set_ui(half, 1, 2);
	for(unsigned long i = MIN; i <= MAX; ++i){
		mpq_init(iss[i - MIN]);
		mpq_set_ui(iss[i - MIN], 1, i*i);
		mpq_init(rss[i - MIN]);
	}
	mpq_set(rss[MAX - MIN], iss[MAX - MIN]);
	for(unsigned long i = MAX - 1; i >= MIN; --i){
		mpq_add(rss[i - MIN], iss[i - MIN], rss[i + 1 - MIN]);
	}
	printf("%f\n", mpq_get_d(rss[0]));
}

void __attribute__((destructor)) dtor(void){
	for(unsigned long i = MIN; i <= MAX; ++i){
		mpq_clear(iss[i - MIN]);
		mpq_clear(rss[i - MIN]);
	}
}

bool findFirstGreedy(const mpq_t s, const mpq_t *iss, const mpq_t *rss, bool *sctd, size_t l){
	if(mpq_equal(s, zero)){
		return true;
	}
	mpq_t ns;
	mpq_init(ns);
	for(size_t i = 0; i < l; ++i){
		if(mpq_cmp(s, iss[i]) >= 0){
			if(mpq_cmp(s, rss[i]) > 0){
				return false;
			}
			mpq_sub(ns, s, iss[i]);
			sctd[i] = true;
			if(findFirstGreedy(ns, iss + i + 1, rss + i + 1, sctd + i + 1, l - i - 1)){
				return true;
			}
			sctd[i] = false;
		}
	}
	mpq_clear(ns);
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

