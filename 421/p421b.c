#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <primesieve.h>

#include "../util/factorization.h"
#include "../util/polynomial.h"

const uint64_t max_n = 100000000000;
const uint64_t max_p = 100000000;

//#{k: a <= kp + r <= b}
// = #{k: a - r <= kp <= b - r}
// = #{k: ceil((a - r)/p) <= k <= floor((b - r)/p)}
// = floor((b - r)/p) + 1 - ceil((a - r)/p)
static inline uint64_t num_residues_range(uint64_t r, uint64_t p, uint64_t a, uint64_t b){
	uint64_t l = (a + p - 1 - r)/p;
	uint64_t u = (b - r)/p;
	return u + 1 - l;
}

int main(){
	poly_t f4[1], f8[1];
	if(!init_poly(f4, 5)){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate polynomials.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	if(!init_poly(f8, 9)){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate polynomials.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	f4->coeffs[0] = 1;
	f4->coeffs[1] = -1;
	f4->coeffs[2] = 1;
	f4->coeffs[3] = -1;
	f4->coeffs[4] = 1;
	f4->len = 5;
	
	f8->coeffs[0] = 1;
	f8->coeffs[1] = 1;
	f8->coeffs[2] = 0;
	f8->coeffs[3] = -1;
	f8->coeffs[4] = -1;
	f8->coeffs[5] = -1;
	f8->coeffs[6] = 0;
	f8->coeffs[7] = 1;
	f8->coeffs[8] = 1;
	f8->len = 9;
	
	size_t num_primes;
	fprintf(stderr, "\e[1;34mGenerating primes < 10**8...\e[0m\n");
	uint64_t *primes = primesieve_generate_primes(0, max_p, &num_primes, UINT64_PRIMES);
	if(!primes){
		fprintf(stderr, "\e[1;31mERROR: Could not generate primes.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\e[1;34mFound %zu primes.\nFinding sum of n < 10**11 where p|n+1, p|n**2-n+1, p|n**4-n**3+n**2-n+1, and p|n**8+n**7-n**5-n**4-n**3+n+1.\e[0m\n", num_primes);
	uint64_t s = 0;
	#pragma omp parallel reduction(+:s)
	{
		poly_t tmp_polys[6];
		for(uint64_t i = 0; i < 6; ++i){
			if(!init_poly(tmp_polys + i, 10)){
				fprintf(stderr, "\e[1;31mERROR: Could not allocate polynomials.\e[0m\n");
				exit(EXIT_FAILURE);
			}
		}
		
		poly_roots_t roots[1];
		if(!init_poly_roots(roots, 8)){
			fprintf(stderr, "\e[1;31mERROR: Could not allocate polynomial root list.\e[0m\n");
			exit(EXIT_FAILURE);
		}
		
		#pragma omp for
		for(size_t i = 0; i < num_primes; ++i){
			uint64_t p = primes[i];
			//printf(" p=%"PRIu64"\n", p);
			//sum of n so that p | n + 1
			//n = -1 mod p
			s += p*num_residues_range(p - 1, p, 1, max_n);
			//sum of n so that p | n**2 - n + 1
			//4*n**2 - 4*n + 4 = 0 mod p
			//(2*n - 1)**2 = -3 mod p
			//n = ((p + 1)/2)*(1 +- sqrt(-3)) mod p
			//p = 2 (no solutions) or 3 (n = 2 mod 3) or
			//(-3/p) = 1 = (p/3) -> p = 1 mod 3
			if(p == 3){
				s += 3*num_residues_range(2, 3, 1, max_n);
			}else if(p%3 == 1){
				uint64_t t = sqrt_mod(p - 3, p);
				uint64_t r = ((p + 1)/2)*(1 + t)%p;
				s += p*num_residues_range(r, p, 1, max_n);
				r = ((p + 1)/2)*(p + 1 - t)%p;
				s += p*num_residues_range(r, p, 1, max_n);
			}
			//sum of n so that p | n**4 - n**3 + n**2 - n + 1
			if(p%5 <= 1){
				if(!roots_poly_modn(f4, p, roots, tmp_polys)){
					fprintf(stderr, "\e[1;31mERROR: \"roots_poly_modn\" failed for p=%"PRId64".\e[0m\n", p);
					exit(EXIT_FAILURE);
				}
				for(uint64_t j = 0; j < roots->len; ++j){
					s += p*num_residues_range(roots->roots[j], p, 1, max_n);
				}
			}
			//sum of n so that p | n**8 + n**7 - n**5 - n**4 - n**3 + n + 1
			if(p%15 <= 1){
				if(!roots_poly_modn(f8, p, roots, tmp_polys)){
					fprintf(stderr, "\e[1;31mERROR: \"roots_poly_modn\" failed for p=%"PRId64".\e[0m\n", p);
					exit(EXIT_FAILURE);
				}
				for(uint64_t j = 0; j < roots->len; ++j){
					s += p*num_residues_range(roots->roots[j], p, 1, max_n);
				}
			}
		}
		destroy_poly_roots(roots);
		for(uint64_t i = 0; i < 6; ++i){
			destroy_poly(tmp_polys + i);
		}
	}
	//subtract sum of prime divisor sums for gcd(f1, f2)
	//3 | gcd(f1, f2) -> 3 | n + 1 -> n = -1 mod 3
	s -= 3*num_residues_range(2, 3, 1, max_n);
	//subtract sum of prime divisor sums for gcd(f1, f4)
	//5 | gcd(f1, f4) -> 5 | n + 1 -> n = -1 mod 5
	s -= 5*num_residues_range(4, 5, 1, max_n);
	//gcd(f2, f8) = gcd(5, n**2 - n + 1), but we can check f2 has no solutions mod 5 so this is 1
	//gcd(f4, f8) = gcd(3, n**4 - n**3 + n**2 - n + 1), but we cah check f4 has no solutions mod 3 so this is 1
	//the other inclusion/exclusion sets ({f1,f8}, {f2,f4}, {f2,f4,f8}, {f1,f4,f8}, {f1,f2,f8}, {f1,f2,f4}, and {f1,f2,f4,f8}) have gcd 1
	printf("s=%"PRIu64"\n", s);
	primesieve_free(primes);
	destroy_poly(f4);
	destroy_poly(f8);
}

