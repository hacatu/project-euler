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

uint64_t max_mmi_ccrt_rec(uint64_t max_mmi, uint64_t num_primes, uint64_t i, const uint64_t partial_products[restrict static num_primes], uint64_t partial_lifts[restrict static num_primes]){
	if(i == num_primes){
		if(max_mmi*max_mmi%partial_products[i - 1] != 1){
			fprintf(stderr, "\e[1;31mERROR: Cartesian CRT produced bad value (%"PRIu64") for %"PRIu64"\e[0m\n", max_mmi, partial_products[i - 1]);
		}
		if(partial_lifts[i - 1] == partial_products[i - 1] - 1){
			return max_mmi;
		}
		return partial_lifts[i - 1] > max_mmi ? partial_lifts[i - 1] : max_mmi;
	}
	uint64_t pk = partial_products[i]/partial_products[i-1];
	partial_lifts[i] = lift_crt(partial_lifts[i-1], partial_products[i-1], 1, pk);
	max_mmi = max_mmi_ccrt_rec(max_mmi, num_primes, i + 1, partial_products, partial_lifts);
	partial_lifts[i] = lift_crt(partial_lifts[i-1], partial_products[i-1], pk-1, pk);
	return max_mmi_ccrt_rec(max_mmi, num_primes, i + 1, partial_products, partial_lifts);
}

uint64_t max_mmi_ccrt(const factors_t *buf){
	uint64_t num_primes = buf->num_primes;
	uint64_t partial_products[num_primes];
	uint64_t partial_lifts[num_primes];
	for(uint64_t i = 0, q = 1; i < num_primes; ++i){
		partial_products[i] = q *= pow_u64(buf->factors[i].prime, buf->factors[i].power);
	}
	uint64_t max_mmi = 1;
	partial_lifts[0] = 1;
	max_mmi = max_mmi_ccrt_rec(max_mmi, num_primes, 1, partial_products, partial_lifts);
	if(partial_products[0] != 2){
		partial_lifts[0] = partial_products[0] - 1;
		max_mmi = max_mmi_ccrt_rec(max_mmi, num_primes, 1, partial_products, partial_lifts);
		if(partial_products[0]%8 == 0){
			for(uint64_t r = partial_products[0]/2 - 1; r <= partial_products[0]/2 + 1; r += 2){
				partial_lifts[0] = r;
				max_mmi = max_mmi_ccrt_rec(max_mmi, num_primes, 1, partial_products, partial_lifts);
			}
		}
	}
	return max_mmi;
}

uint64_t max_mmi_gcrt(uint64_t n, const factors_t *buf){
	//we can find the mmi independently for every prime power in the factorization of n.  powers of 2 work differently though
	//2 has no effect because 1 and -1 are the same.  4 acts the same as any odd prime, but 8 and higher powers have 4 square roots of 1
	//for k >= 0, 2^(3+k) has roots 1, 2^(2+k)-1, 2^(2+k)+1, 2^(3+k)-1
	//for 2^(3+k)q, let p(a,b) be the isomorphism from Z/2^(3+k) x Z/q to Z/2^(3+k)q.  p(1,b) = -p(-1,-b) = 2^(k+2)q-p(2^(k+2)-1,-b) = 2^(k+2)q+p(2^(k+2)+1,b)
	//we account for the special case of 2 with `p2_offset` and the case of 8 and higher powers with `contains_Z_8`
	//so we can think of any square root of 1 as p(+1,-1) where p maps from Z/p x Z/q to Z/pq where p and q are coprime (but not necessarily prime) factors of
	//n.  this means we only need to apply CRT once, we just have to multiply the prime powers for which the supposed square root is 1 together to get p and multiply
	//the prime powers for which it is -1 together to get q.  This is accelerated by using gray code so we visit all choices of p and q aside from q = 1 and only
	//have to divide and multiply by a single prime power.
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
		if(r*r%(n << contains_Z_8) != 1){
			fprintf(stderr, "\e[1;31mERROR: Gray code CRT produced bad value (%"PRIu64") for %"PRIu64"\e[0m\n", r, n << contains_Z_8);
		}
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
	uint64_t r = factor_heuristic(n, num_primes, primes, &factor_conf, buf);
	if(r != 1){
		fprintf(stderr, "\e[1;31mERROR: Factorization failed on %"PRIu64"\e[0m\n", n);
		return 0;
	}
	//uint64_t m_ccrt = max_mmi_ccrt(buf);
	return max_mmi_gcrt(n, buf);
	/*if(m_ccrt != m_gcrt){
		fprintf(stderr, "\e[1;31mERROR: mismatch for %"PRIu64": m_ccrt=%"PRIu64", m_gcrt=%"PRIu64"\e[0m\n", n, m_ccrt, m_gcrt);
		return 0;
	}*/
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

