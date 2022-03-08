#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <primesieve.h>

#include "../util/factorization.h"

factor_conf_t factor_conf = {
	.pollard_max=0,
	.pollard_stride=100,
	.lenstra_max=UINT64_MAX,
	.lenstra_bfac=10,
	.qsieve_max=0
};

uint64_t max_mmi_gcrt(uint64_t n, const factors_t *buf){
	uint64_t num_primes = buf->num_primes;
	uint64_t p2_offset = __builtin_ctzll(n) == 1;
	int contains_Z_8 = __builtin_ctzll(n) >= 3;
	n >>= contains_Z_8;
	if(num_primes - p2_offset <= 1){
		return contains_Z_8 ? n + 1 : 1;
	}
	uint64_t powers[num_primes];
	uint64_t pos_product = powers[0] = pow_u64(buf->factors[0].prime, buf->factors[0].power) >> contains_Z_8;
	for(uint64_t i = 2; i < num_primes; ++i){
		pos_product *= powers[i] = pow_u64(buf->factors[i].prime, buf->factors[i].power);
	}
	uint64_t neg_product = powers[1] = pow_u64(buf->factors[1].prime, buf->factors[1].power);
	uint64_t n_cur = 1;
	uint64_t g_prev = 0, g_cur = 1;
	uint64_t max_mmi = 1;
	while(1){
		uint64_t r = lift_crt(1, pos_product, neg_product - 1, neg_product);
		r = r > n - r ? r : n - r;
		max_mmi = r > max_mmi ? r : max_mmi;
		if(++n_cur == (1ull << (num_primes - p2_offset - 1))){
			break;
		}
		g_prev = g_cur;
		g_cur = n_cur ^ (n_cur >> 1);
		uint64_t t = g_prev ^ g_cur;
		uint64_t i = __builtin_ctzll(t) + 1;
		if(g_cur & t){
			neg_product *= powers[i];
			pos_product /= powers[i];
		}else{
			pos_product *= powers[i];
			neg_product /= powers[i];
		}
	}
	if(contains_Z_8){
		max_mmi += n;
	}
	return max_mmi;
}

int test_single_max_mmi(uint64_t n, uint64_t num_primes, const uint64_t primes[restrict static num_primes], factors_t *buf){
	factor_heuristic(n, num_primes, primes, &factor_conf, buf);
	return max_mmi_gcrt(n, buf);
}

int main(){
	size_t num_primes;
	uint64_t *primes = primesieve_generate_primes(1, 1000, &num_primes, UINT64_PRIMES);
	uint64_t s = 0;
	#pragma omp parallel reduction(+:s)
	{
		factors_t *factors = init_factors_t_ub((1ull << 30) + 3, num_primes, primes);
		#pragma omp for
		for(uint64_t n = 3; n <= 20000000; ++n){
			s += test_single_max_mmi(n, num_primes, primes, factors);
		}
		free(factors);
	}
	primesieve_free(primes);
	printf("%"PRIu64"\n", s);
}

