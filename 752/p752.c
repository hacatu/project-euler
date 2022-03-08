#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <crater/hash.h>
#include <nut/sieves.h>
#include <nut/modular_math.h>

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

int add_qfp(cr8r_base_ft *ft, void *_a, void *_b){
	uint64_t *a = _a, *b = _b;
	if(a[2] > b[2]){
		a[2] = b[2];
	}
	return 1;
}

int cmp_qfp(const cr8r_base_ft *ft, const void *_a, const void *_b){
	return memcmp(_a, _b, 2*sizeof(uint64_t));
}

uint64_t hash_qfp(const cr8r_base_ft *_ft, const void *_a){
	cr8r_base_ft ft = {.size=2*sizeof(uint64_t)};
	return cr8r_default_hash(&ft, _a);
}

cr8r_hashtbl_ft qfpft = {
	.base.size = 3*sizeof(uint64_t),
	.add = add_qfp,
	.cmp = cmp_qfp,
	.hash = hash_qfp,
	.load_factor = .5
};

uint64_t qf_dlog1(cr8r_hashtbl_t *pows, uint64_t x){
	// We want to find the minimal n st (1 + sqrt(7))**n = 1 mod x.
	// To do this, first notice that by the pidgeonhole principle
	// there must be some repeat within the sequence if n ranges
	// from 0 to x**2, because there are only x**2 possible
	// a + b*sqrt(7) mod x.  Then we can write
	// ((-6)^-1*(1 - sqrt(7)))**(a*x) = (1 + sqrt(7))**b mod x
	// and express n = a*x + b and find the smallest a for which
	// a matching b exists.
	cr8r_hash_clear(pows, &qfpft);
	uint64_t res = 0;
	uint64_t z[] = {1, 1};
	int status;
	for(uint64_t p[] = {1, 1, 1}; p[2] < x; qf_imulmod(p, z, x), ++p[2]){
		if(p[0] == 1 && p[1] == 0){
			return p[2];
		}
		cr8r_hash_append(pows, &qfpft, p, &status);
		if(!status){
			fprintf(stderr, "\e[1;31mOut of memory!\e[0m\n");
		}
	}
	cr8r_hash_append(pows, &qfpft, (uint64_t[3]){1, 0, 0}, &status);
	uint64_t i6 = (x + 1)/2;
	if(x%3 == 2){
		i6 = i6*(x + 1)/3%x;
	}else{
		i6 = i6*(2*x + 1)/3%x;
	}
	z[0] = x - i6;
	z[1] = i6;
	qf_powmod(z, z, x, x);
	for(uint64_t p[] = {z[0], z[1], 1}; p[2] < x; qf_imulmod(p, z, x), ++p[2]){
		uint64_t *r = cr8r_hash_get(pows, &qfpft, p);
		if(r){
			res = p[2]*x + r[2];
			break;
		}
	}
	return res;
}

int main(){
	/*uint64_t w = 0;
	void *factorss = sieve_factorizations(100, &w);
	size_t pitch;
	{
		factors_t dummy;
		pitch = offsetof(factors_t, factors) + w*sizeof(*dummy.factors);
	}
	for(uint64_t x = 5; x <= 100; x += 4){
		uint64_t z[] = {1, 1};
		uint64_t l = carmichael_lambda(pitch_arr_get(factorss, pitch, x));
		qf_powmod(z, z, l, x);
		printf("(1 + sqrt(7))^%"PRIu64" mod %"PRIu64" = %"PRIu64" + %"PRIu64"sqrt(7)\n", l, x, z[0], z[1]);
		x += 2;
		if(x > 100){
			break;
		}
		z[0] = z[1] = 1;
		l = carmichael_lambda(pitch_arr_get(factorss, pitch, x));
		qf_powmod(z, z, l, x);
		printf("(1 + sqrt(7))^%"PRIu64" mod %"PRIu64" = %"PRIu64" + %"PRIu64"sqrt(7)\n", l, x, z[0], z[1]);
	}
	*/
	uint64_t N = 100000;
	cr8r_hashtbl_t pows = {};
	if(!cr8r_hash_init(&pows, &qfpft, 2000009)){
		fprintf(stderr, "\e[1;31mOut of memory!\e[0m\n");
		exit(EXIT_FAILURE);
	}
	uint64_t s = 0;
	for(uint64_t x = 5; x <= N; x += 4){
		uint64_t n = qf_dlog1(&pows, x);
		/*uint64_t z[] = {1, 1};
		qf_powmod(z, z, n, x);
		printf("z^%"PRIu64" mod %"PRIu64" = %"PRIu64" + %"PRIu64"√7\n", n, x, z[0], z[1]);*/
		//printf("%"PRIu64"\n", x);
		s += n;
		x += 2;
		if(x > N){
			break;
		}
		n = qf_dlog1(&pows, x);
		/*z[0] = z[1] = 1;
		qf_powmod(z, z, n, x);
		printf("z^%"PRIu64" mod %"PRIu64" = %"PRIu64" + %"PRIu64"√7\n", n, x, z[0], z[1]);*/
		s += n;
	}
	printf("G(%"PRIu64")=%"PRIu64"\n", N, s);
	cr8r_hash_destroy(&pows, &qfpft);
}

