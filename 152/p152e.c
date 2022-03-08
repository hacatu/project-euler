#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <primesieve.h>
#include <gmp.h>

typedef struct{
	mpz_t mod;
	mpz_t (*sets)[];
	size_t len, cap;
} group_t;

typedef struct{
	group_t table[4093];
	group_t **items;
	size_t len;
} map_t;

const uint64_t MAX = 80;
const uint64_t TARGET = 2;

uint64_t *basis;
size_t basis_len;
uint64_t **orths;
size_t *orth_lens;
mpz_t denom;
mpz_t *target_basis;
map_t *agroups, *bgroups;

uint64_t maxexp_lt(uint64_t n, uint64_t m){
	uint64_t r = 0;
	for(; m >= n; m /= n, ++r);
	return r;
}

uint64_t maxpow_lt(uint64_t n, uint64_t m){
	uint64_t r = 1;
	for(m /= n; r <= m; r *= n);
	return r;
}

uint64_t maxpow_in(uint64_t n, uint64_t m){
	uint64_t r = 1;
	for(; m%n == 0; m /= n, r *= n);
	return r;
}

void maxpow_in_mpz(mpz_t out, mpz_t n, mpz_t m){
	mpz_remove(out, m, n);
	mpz_divexact(out, m, out);
}

int cmp_uint64_ts(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return +1;
	}
	return 0;
}

int init_basis(void){
	basis = primesieve_generate_primes(0, MAX + 1, &basis_len, UINT64_PRIMES);
	do{
		LOOP_PRUNE_BASIS:;
		for(size_t i = basis_len; i-- > 0;){
			uint64_t p = basis[i], prod = 1;
			for(size_t j = 0; j < basis_len; ++j){
				uint64_t fact = maxpow_lt(basis[j], MAX/p);
				prod *= fact*fact;
			}
			if(TARGET%p){
				if(prod < p*p){
					memmove(basis + i, basis + i + 1, (basis_len-- - i)*sizeof(uint64_t));
					goto LOOP_PRUNE_BASIS;
				}
			}else if(prod < p){
				memmove(basis + i, basis + i + 1, (basis_len-- - i)*sizeof(uint64_t));
				goto LOOP_PRUNE_BASIS;
			}
		}
	}while(0);
	return 1;
}

int init_orths(void){
	uint64_t p = basis[0];
	orths = malloc(basis_len*sizeof(uint64_t*));
	orth_lens = calloc(basis_len, sizeof(size_t));
	orth_lens[0] = maxexp_lt(p, MAX);
	orths[0] = malloc(orth_lens[0]*sizeof(uint64_t));
	for(uint64_t i = 0, n = 1; n <= MAX/p; n *= p, ++i){
		orths[0][i] = n;
	}
	for(uint64_t i = 1, len; i < basis_len; ++i){
		p = basis[i];
		len = 0;
		orths[i] = malloc(MAX/p*sizeof(uint64_t));
		for(uint64_t j = 0, n; j < orth_lens[i - 1]; ++j){
			n = orths[i - 1][j];
			for(uint64_t m = n; m <= MAX/p; m *= p){
				orths[i][len++] = m;
			}
		}
		orth_lens[i] = len;
		orths[i] = realloc(orths[i], len*sizeof(uint64_t));
		qsort(orths[i], len, sizeof(uint64_t), cmp_uint64_ts);
	}
	return 1;
}

int init_denom(void){
	mpz_t fact, p;
	mpz_inits(fact, p, NULL);
	mpz_init_set_ui(denom, 1);
	for(size_t i = 0; i < basis_len; ++i){
		mpz_ui_pow_ui(fact, maxpow_lt(basis[i], MAX), 2);
		mpz_mul(denom, denom, fact);
	}
	target_basis = malloc(basis_len*sizeof(mpz_t));
	for(size_t i = 0; i < basis_len; ++i){
		mpz_set_ui(p, basis[i]);
		mpz_init(target_basis[i]);
		maxpow_in_mpz(target_basis[i], p, denom);
		mpz_divexact_ui(target_basis[i], target_basis[i], maxpow_in(basis[i], TARGET));
	}
	mpz_clears(fact, p, NULL);
	return 1;
}

int group_append(map_t *s, mpz_t mod, const mpz_t *msums, int make_igroups){
	size_t i = mpz_fdiv_ui(mod, 4093);
	for(; s->table[i].len && mpz_cmp(s->table[i].mod, mod); i = (i + 1)%4093);
	group_t *group = s->table + i;
	mpz_t (*sets)[basis_len + 1] = group->sets;
	if(group->len + 1 > group->cap){
		if(group->cap){
			sets = realloc(sets, group->cap*2*sizeof(*sets));
			group->sets = sets;
			group->cap *= 2;
		}else{
			mpz_init_set(group->mod, mod);
			if(make_igroups){
				s->items[s->len++] = group;
			}
			sets = malloc(1*sizeof(*sets));
			group->sets = sets;
			group->cap = 1;
		}
	}
	for(size_t i = 0; i < basis_len + 1; ++i){
		mpz_init_set(sets[group->len][i], msums[i]);
	}
	++group->len;
	return 1;
}

int gen_groups(map_t *out, uint64_t *base, size_t base_len, size_t i, int make_igroups){
	int64_t k = 0;
	mpz_t msums[basis_len + 1], mod, delta;
	mpz_inits(mod, delta, NULL);
	for(size_t i = 0; i < basis_len + 1; ++i){
		mpz_init_set_ui(msums[i], 0);
	}
	uint64_t gcode = 0, end = 1UL << base_len;
	map_t *s = out + i;
	if(make_igroups){
		s->items = malloc(end*sizeof(group_t*));
	}
	while(base_len){
		mpz_fdiv_r(mod, msums[i], target_basis[i]);
		group_append(s, mod, msums, make_igroups);
		++k;
		uint64_t xbit = k&-k;
		gcode ^= xbit;
		if(gcode >= end){
			return 1;
		}
		size_t xi = __builtin_ctzll(xbit);
		mpz_set_ui(delta, basis[i]);
		mpz_mul_ui(delta, delta, base[xi]);
		mpz_pow_ui(delta, delta, 2);
		mpz_divexact(delta, denom, delta);
		if(!(gcode&xbit)){
			mpz_neg(delta, delta);
		}
		mpz_add(msums[basis_len], msums[basis_len], delta);
		mpz_add(msums[i], msums[i], delta);
		for(size_t j = 0; j < i; ++j){
			if(base[xi]%basis[j] == 0){
				mpz_add(msums[j], msums[j], delta);
			}
		}
	}
	return 1;
}

int init_groups(void){
	agroups = calloc(basis_len, sizeof(*agroups));
	bgroups = calloc(basis_len, sizeof(*bgroups));
	for(size_t i = 0; i < basis_len; ++i){
		uint64_t *orth = orths[i];
		size_t len = orth_lens[i];
		gen_groups(agroups, orth, len/2, i, 1);
		gen_groups(bgroups, orth + len/2, (len + 1)/2, i, 0);
	}
	return 1;
}

uint64_t match_groups_rec(int64_t i, const mpz_t *msums){
	mpz_t bmod;
	mpz_init(bmod);
	if(i < 0){
		mpz_divexact_ui(bmod, denom, TARGET);
		return !mpz_cmp(msums[basis_len], bmod);
	}
	mpz_t asums[basis_len + 1];
	mpz_t bsums[basis_len + 1];
	for(size_t i = 0; i < basis_len + 1; ++i){
		mpz_inits(asums[i], bsums[i], NULL);
	}
	uint64_t ret = 0;
	for(size_t j = 0; j < agroups[i].len; ++j){
		group_t *agroup = agroups[i].items[j];
		mpz_add(bmod, agroup->mod, msums[i]);
		mpz_neg(bmod, bmod);
		mpz_fdiv_r(bmod, bmod, target_basis[i]);
		size_t k = mpz_fdiv_ui(bmod, 4093);
		for(; bgroups[i].table[k].len && mpz_cmp(bgroups[i].table[k].mod, bmod); k = (k + 1)%4093);
		group_t *bgroup = bgroups[i].table + k;
		for(size_t ib = 0; ib < bgroup->len; ++ib){
			//printf("%"PRIi64"\n", i);
			mpz_t *bset = ((mpz_t (*)[basis_len + 1])bgroup->sets)[ib];
			for(size_t i = 0; i < basis_len + 1; ++i){
				mpz_add(bsums[i], msums[i], bset[i]);
			}
			for(size_t ia = 0; ia < agroup->len; ++ia){
				mpz_t *aset = ((mpz_t (*)[basis_len + 1])agroup->sets)[ia];
				for(size_t i = 0; i < basis_len + 1; ++i){
					mpz_add(asums[i], bsums[i], aset[i]);
				}
				ret += match_groups_rec(i - 1, asums);
			}
		}
	}
	return ret;
}

int main(void){
	init_basis();
	init_orths();
	init_denom();
	init_groups();
	mpz_t msums[basis_len + 1];
	for(size_t i = 0; i < basis_len + 1; ++i){
		mpz_init_set_ui(msums[i], 0);
	}
	printf("%"PRIu64"\n", match_groups_rec(basis_len - 1, msums));
}

