#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <primesieve.h>
#include "sla.h"
#include "avl.h"

#define MIN_FACTORS 1000000ull
#define NUM_COUNT 1000000ull
#define NUM_MOD 123454321ull

typedef struct prime_factor prime_factor;
struct prime_factor{
	uint32_t prime, power, index;
	prime_factor *next;
};

typedef struct{
	double log;
	prime_factor first;
} number;

int cmp_numbers_by_log(const void *_a, const void *_b){
	static const double log_epsilon = (log(15485653.) - log(15485651.))/8.;//this is the last twin prime pair in the first million primes
	double a = ((const number*)_a)->log, b = ((const number*)_b)->log;
	if(fabs(a - b) < log_epsilon){
		return 0;
	}
	return a < b ? -1 : 1;
}

void dummy_free(void *alloc_data, void *p){}

uint64_t powmod(unsigned __int128 b, uint64_t e, uint64_t n){
	unsigned __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return (uint64_t)r;
}

avl_ft number_fns = {
	.size= sizeof(number),
	.cmp= cmp_numbers_by_log,
	.alloc= (void*(*)(void*))sla_alloc,
	.free= dummy_free,
};

int main(){
	uint32_t *primes = primesieve_generate_n_primes(NUM_COUNT, 0, UINT32_PRIMES);
	if(!primes){
		printf("\e[1;31mERROR: could not generate prime numbers\e[0m\n");
		return 1;
	}
	slab_allocator pfsla, numbersla;
	if(!sla_init(&pfsla, sizeof(prime_factor), 0x400000ull)){
		printf("\e[1;31mERROR: could not reserve initial space for prime factor data\e[0m\n");
		primesieve_free(primes);
		return 1;
	}
	if(!sla_init(&numbersla, offsetof(avl_node, data) + sizeof(number), 0x400000ull)){
		printf("\e[1;31mERROR: could not reserve initial space for avl tree nodes\e[0m\n");
		sla_delete(&pfsla);
		primesieve_free(primes);
		return 1;
	}
	number_fns.alloc_data = &numbersla;
	avl_node *numbers = avl_new(&(number){MIN_FACTORS*log(2.), {2, MIN_FACTORS, 0, NULL}}, NULL, NULL, NULL, 0, &number_fns);
	if(!numbers){
		printf("\e[1;31mERROR: could not allocate root avl node\e[0m\n");
		sla_delete(&pfsla);
		sla_delete(&numbersla);
		primesieve_free(primes);
		return 1;
	}
	const number *num;
	for(uint32_t i = 0; i < NUM_COUNT; ++i){
		avl_node *first = avl_first(numbers);
		numbers = avl_remove_node(first, &number_fns);
		num = (const number*)first->data;
		//printf("Generating numbers adjacent to e^%f\n", num->log);
		for(const prime_factor *pf = &num->first; pf; pf = pf->next){
			uint32_t index = pf->index + 1;
			if(index == NUM_COUNT){
				break;
			}
			uint32_t p_i_1 = primes[index];
			number tmp = {num->log + log((double)p_i_1/pf->prime)};
			const prime_factor *it = &num->first;
			prime_factor *ot = &tmp.first;
			for(; it != pf; it = it->next){
				*ot = *it;
				if(!(ot = ot->next = sla_alloc(&pfsla))){
					printf("\e[1;31mERROR: could not allocate additional prime factor object\e[0m\n");
					sla_delete(&pfsla);
					sla_delete(&numbersla);
					primesieve_free(primes);
					return 1;
				}
			}
			if(it->power > 1){
				*ot = *it;
				--ot->power;
				if(!(ot = ot->next = sla_alloc(&pfsla))){
					printf("\e[1;31mERROR: could not allocate additional prime factor object\e[0m\n");
					sla_delete(&pfsla);
					sla_delete(&numbersla);
					primesieve_free(primes);
					return 1;
				}
			}
			it = it->next;
			if(it && it->prime == p_i_1){
				*ot = *it;
				++ot->power;
			}else{
				*ot = (prime_factor){p_i_1, 1, index, (prime_factor*)it};
			}
			avl_insert(&numbers, &tmp, &number_fns);//can't detect errors since this version of the avl tree library is a lil busted
		}
		number tmp = {num->log + log(2), num->first};
		avl_delete(first, &number_fns);//maybe first isn't unlinked all the way?
		++tmp.first.power;
		avl_insert(&numbers, &tmp, &number_fns);//again we can't detect errors
	}
	primesieve_free(primes);
	uint64_t res = 1;
	for(const prime_factor *it = &num->first; it; it = it->next){
		res = res*powmod(it->prime, it->power, NUM_MOD)%NUM_MOD;
	}
	sla_delete(&pfsla);
	sla_delete(&numbersla);
	printf("%"PRIu64"\n", res);
}

