#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nut/sieves.h>

uint64_t w;

void qf_isqmod(uint64_t z[static 2], uint64_t x){
	uint64_t a = (z[0]*z[0] + 7*z[1]*z[1])%x;
	uint64_t b = 2*z[0]*z[1]%x;
	z[0] = a;
	z[1] = b;
}

void qf_imulmod(uint64_t z1[static 2], const uint64_t z2[static 2], uint64_t x){
	uint64_t a = (z1[0]*z2[0] + 7*z1[1]*z2[1])%x;
	uint64_t b = (z1[1]*z2[0] + z1[0]*z2[1])%x;
	z1[0] = a;
	z1[1] = b;
}

void qf_powmod(uint64_t out[static 2], const uint64_t z[static 2], uint64_t n, uint64_t x){
	uint64_t s[] = {z[0], z[1]};
	out[0] = 1;
	out[1] = 0;
	while(n){
		if(n&1){
			qf_imulmod(out, s, x);
		}
		qf_isqmod(s, x);
		n >>= 1;
	}
}

uint64_t ord_zp(const factors_t *factors, uint64_t x){
	factors_t *dfactors = copy_factors_t(factors);
	uint64_t d = x - 1;
	for(uint64_t i = 0; i < factors->num_primes; ++i){
		while(dfactors->factors[i].power){
			uint64_t d1 = d/factors->factors[i].prime;
			if(powmod(x - 6, d1, x) != 1){
				break;
			}
			d = d1;
			--dfactors->factors[i].power;
		}
	}
	free(dfactors);
	return d;
}

uint64_t qf_dlog1_baby(uint64_t x, const void *factorss, size_t pitch){
	const factors_t *factors = pitch_arr_get((void*)factorss, pitch, x - 1);
	factors_t *dfactors = NULL;
	uint64_t y = ord_zp(factors, x);
	uint64_t n = x - 1;
	// y | g(p) | p - 1 or p^2 - 1
	switch(x%28){
		case 1: case 3: case 9: case 27: case 25: case 19:
			dfactors = copy_factors_t(pitch_arr_get((void*)factorss, pitch, n/y));
			break;
		case 7:
			return 7;
		default:
			dfactors = init_factors_t_w(w);
			memcpy(dfactors, pitch_arr_get((void*)factorss, pitch, n/y), pitch);
			n *= x + 1;
			factors_combine(dfactors, pitch_arr_get((void*)factorss, pitch, x + 1), 1);
	}
	for(uint64_t i = 0; i < dfactors->num_primes; ++i){
		while(dfactors->factors[i].power){
			uint64_t d1 = n/dfactors->factors[i].prime;
			uint64_t z[] = {1, 1};
			qf_powmod(z, z, d1, x);
			if(memcmp(z, (uint64_t[2]){1, 0}, sizeof(z))){
				break;
			}
			n = d1;
			--dfactors->factors[i].power;
		}
	}
	free(dfactors);
	return n;
}

uint64_t qf_dlog1(uint32_t *dlogs, uint64_t x, const void *factorss, size_t pitch){
	const factors_t *factors = pitch_arr_get((void*)factorss, pitch, x);
	if(factors->num_primes == 1 && factors->factors->power == 1){
		return dlogs[x] = qf_dlog1_baby(x, factorss, pitch);
	}else if(factors->num_primes == 1){
		uint64_t p = factors->factors->prime;
		return dlogs[x] = p*dlogs[x/p];
	}else{
		uint64_t p = pow_u64(factors->factors->prime, factors->factors->power);
		return dlogs[x] = lcm(dlogs[p], dlogs[x/p]);
	}
}

int main(){
	uint64_t N = 1000000;
	uint64_t s = 0;
	void *factorss = sieve_factorizations(N, &w);
	size_t pitch;
	{
		factors_t dummy;
		pitch = offsetof(factors_t, factors) + w*sizeof(*dummy.factors);
	}
	uint32_t *dlogs = malloc(N*sizeof(uint32_t));
	if(!dlogs){
		fprintf(stderr, "\e[1;31mOut of memory!\e[0m\n");
		exit(EXIT_FAILURE);
	}
	for(uint64_t x = 5; x <= N; x += 4){
		s += qf_dlog1(dlogs, x, factorss, pitch);
		x += 2;
		if(x > N){
			break;
		}
		s += qf_dlog1(dlogs, x, factorss, pitch);
	}
	printf("G(%"PRIu64")=%"PRIu64"\n", N, s);
	free(dlogs);
	free(factorss);
}

