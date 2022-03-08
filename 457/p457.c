#include <stdio.h>
#include <inttypes.h>
#include <primesieve.h>
#include <stdlib.h>

//static const uint64_t MAX = 10000000ull;
static const uint64_t MAX = 10000000ull;

signed __int128 powmod(signed __int128 b, signed __int128 e, signed __int128 n){
	signed __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return r;
}

signed __int128 gcd(signed __int128 a, signed __int128 b){
	if(b > a){
		signed __int128 temp = a;
		a = b;
		b = temp;
	}
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

signed __int128 jacobik(signed __int128 n, signed __int128 k){
	signed __int128 j = 1;
	while(1){
		signed __int128 s = __builtin_ctzll(n);
		signed __int128 q = n >> s;
		if((s&1) && ((k&7) == 3 || ((k&7) == 5))){
			j = -j;
		}
		if(q == 1){
			return j;
		}else if(q == k - 1){
			return (k&3)==1 ? j : -j;
		}else if((q&2) && (k&2)){
			j = -j;
		}
		n = k%q;
		k = q;
	}
}

signed __int128 randint(signed __int128 a, signed __int128 b){
	signed __int128 l = b - a, m = RAND_MAX/l*l, r;
	while((r = rand()) >= m);
	return r%l + a;
}

signed __int128 random_nonresidue(signed __int128 p){
	while(1){
		signed __int128 z = randint(2, p - 1);
		if(jacobik(z, p) == -1){
			return z;
		}
	}
}

signed __int128 sqrt_shanks(signed __int128 n, signed __int128 p){
	signed __int128 s = __builtin_ctzll(p-1);
	signed __int128 q = p >> s;//p-1 = q*2^s
	signed __int128 z = random_nonresidue(p);
	//printf("trying \"nonresidue\" %"PRIu64"\n", z);
	signed __int128 m = s;
	signed __int128 c = powmod(z, q, p);
	signed __int128 t = powmod(n, q, p);
	signed __int128 r = powmod(n, (q + 1) >> 1, p);
	while(t != 1){
		signed __int128 i = 1;
		for(signed __int128 s = t*t%p; s != 1; s = s*s%p,++i);
		signed __int128 b = c;
		for(signed __int128 j = 0; j < m - i - 1; ++j){
			b = b*b%p;
		}
		m = i;
		c = b*b%p;
		t = t*c%p;
		r = r*b%p;
	}
	return r;
}

signed __int128 sqrt_mod(signed __int128 n, signed __int128 p){
	signed __int128 r;
	if((p&3) == 3){
		r = powmod(n, (p + 1) >> 2, p);
	}else{//if(m=64-__builtin_clzll(p),s=__builtin_ctzll(p-1),8*m+20 <= s*(s-1))
		r = sqrt_shanks(n, p);
	}//else use cipolla except I won't actually implement it
	return r;
}

signed __int128 mmi(signed __int128 a, signed __int128 b){
	signed __int128 r0 = b, r1 = a;
	signed __int128 s0 = 1, s1 = 0;
	signed __int128 t0 = 0, t1 = 1;
	while(r1){
		int64_t q = r0/r1, t;
		t = r1;
		r1 = r0 - q*r1;
		r0 = t;
		t = s1;
		s1 = s0 - q*s1;
		s0 = t;
		t = t1;
		t1 = t0 - q*t1;
		t0 = t;
	}
	return t0 < 0 ? t0 + b : t0;
}

int main(){
	size_t ps_len;
	signed __int128 sr = 5;
	uint64_t *ps = primesieve_generate_primes(5, MAX, &ps_len, UINT64_PRIMES);
	for(size_t i = 0; i < ps_len; ++i){
		signed __int128 p = ps[i];
		signed __int128 d = 3*((p*p + 1) >> 1)%p;
		signed __int128 h = (d*d + 1)%p;
		signed __int128 x = 0;
		if(h){
			if(jacobik(h, p) == 1){
				x = sqrt_mod(h, p);
			}else{
				continue;
			}
		}
		if((x*x - h)%p){
			printf("%"PRId64": BAD SQRT for %"PRId64"\n", (int64_t)p, (int64_t)h);
		}
		//printf("%"PRId64"^2=%"PRId64" mod %"PRId64"\n", x, (d*d + 1)%p, p);
		signed __int128 r1 = (d + x)%p;
		signed __int128 r2 = (d + p - x)%p;
		//printf("n^2-3n-1=0 mod %"PRId64" for n=%"PRId64",%"PRId64"\n", p, r1, r2);
		if((2*r1 - 3)%p == 0){
			if((r1*(r1 - 3) - 1)%(p*p)){
				r1 = p*p;//IE, r1 is not the minimum
				goto LIFT_R2;
			}
		}else{
			signed __int128 q = mmi(2*r1 - 3, p);
			if((2*r1 - 3)*q%p != 1){
				printf("%"PRId64": BAD INVERSE %"PRId64" OF %"PRId64"\n", (int64_t)p, (int64_t)q, (int64_t)(2*r1 - 3));
			}
			r1 = (r1 - r1*q*(r1 - 3) + q) % (p*p);
			if(r1 < 0){
				r1 += p*p;
			}
		}
		if((r1*(r1 - 3) - 1)%(p*p)){
			printf("%"PRId64": BAD LIFT %"PRId64"\n", (int64_t)p, (int64_t)r1);
			break;
		}else{
			//printf("n^2-3n-1=0 mod %"PRId64"^2 for n=%"PRId64"\n", p, r1);
		}
		LIFT_R2:
		
		if((2*r2 - 3)%p == 0){
			if((r2*(r2 - 3) - 1)%(p*p)){
				r2 = p*p;
				goto FIND_MIN_R;
			}
		}else{
			signed __int128 q = mmi(2*r2 - 3, p);
			if((2*r2 - 3)*q%p != 1){
				printf("%"PRId64": BAD INVERSE %"PRId64" OF %"PRId64"\n", (int64_t)p, (int64_t)q, (int64_t)(2*r2 - 3));
			}
			r2 = (r2 - r2*q*(r2 - 3) + q) % (p*p);
			if(r2 < 0){
				r2 += p*p;
			}
		}
		if((r2*(r2 - 3) - 1)%(p*p)){
			printf("%"PRId64": BAD LIFT %"PRId64"\n", (int64_t)p, (int64_t)r2);
			break;
		}else{
			//printf("n^2-3n-1=0 mod %"PRId64"^2 for n=%"PRId64"\n", p, r2);
		}
		FIND_MIN_R:
		
		if(r2 < r1){
			sr += r2;
		}else if(r1 != p*p){
			sr += r1;
		}
	}
	printf("%"PRId64"\n", (int64_t)sr);
	primesieve_free(ps);
}

