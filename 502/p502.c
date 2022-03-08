#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
f(w+1,h,r) = \sum_{i=0}^{r-1}\begin[cases]f(w,h,i) & i = r \mod 2 \\ g(w, h, i) & i \not = \mod 2 \end[cases] + \sum_{i=r}^{h-1} f(w,h,i))
f(w,h,r) is stored at [r] and g(w,h,r) is stored at [h+r]
*/

void _mat_mulmod(size_t h, const int64_t A[static h][h], const int64_t B[static h][h], int64_t C[static h][h], int64_t p, int64_t c){
	for(size_t i = 0; i < h; ++i){
		for(size_t j = 0; j < h; ++j){
			int64_t cij = 0;
			for(size_t b = 0; b < h; b += c){
				for(size_t k = b; k < b + c && k < h; ++k){
					cij += A[i][k]*B[k][j];
				}
				cij %= p;
			}
			C[i][j] = cij;
		}
	}
}

void mat_ident(size_t h, int64_t A[static h][h]){
	memset(A, 0, h*h*sizeof(int64_t));
	for(size_t i = 0; i < h; ++i){
		A[i][i] = 1;
	}
}

void mat_copy(size_t h, const int64_t A[static h][h], int64_t B[static h][h]){
	memcpy(B, A, h*h*sizeof(int64_t));
}

void mat_powmod(size_t h, const int64_t A[static h][h], int64_t B[static h][h], int64_t n, int64_t p, int64_t c){
	int64_t (*S)[h] = malloc(h*h*sizeof(int64_t));
	int64_t (*Tb1)[h] = malloc(h*h*sizeof(int64_t));
	int64_t (*Tb2)[h] = B;
	int64_t (*Ts)[h] = malloc(h*h*sizeof(int64_t));
	int64_t (*t)[h];
	_mat_mulmod(h, A, A, S, p, c);
	if(!(n&1)){
		mat_ident(h, Tb1);
	}else{
		mat_copy(h, A, Tb1);
	}
	n >>= 1;
	for(; n; n >>= 1){
		printf("(mat_powmod: n=%"PRId64")\n", n);
		if(n&1){
			_mat_mulmod(h, Tb1, S, Tb2, p, c);
			t = Tb1;
			Tb1 = Tb2;
			Tb2 = t;//swap the names of Tb1 and Tb2
		}
		_mat_mulmod(h, S, S, Ts, p, c);
		t = Ts;
		Ts = S;
		S = t;//swap the names of S and Ts
	}
	if(Tb2 == B){
		mat_copy(h, Tb1, B);
		free(Tb1);
	}else{
		free(Tb2);
	}
	free(S);
	free(Ts);
}

int64_t f_horiz(int64_t w, int64_t h, int64_t p, int64_t c){
	
	int64_t (*dW)[2*h] = calloc(4*h*h, sizeof(int64_t));
	
	for(size_t r = 0; r < h; ++r){
		for(size_t i = 0; i < r; ++i){
			if((i&1) == (r&1)){
				dW[r][i] = 1;     //f(w+1, h, r) = ... + f(w, h, i) + ... if i = r mod 2
				dW[r+h][i+h] = 1; //g(w+1, h, r) = ... + g(w, h, i) + ... if i = r mod 2
			}else{
				dW[r][i+h] = 1;   //f(w+1, h, r) = ... + g(w, h, i) + ... if i != r mod 2
				dW[r+h][i] = 1;   //g(w+1, h, r) = ... + f(w, h, i) + ... if i != r mod 2
			}
		}
		for(size_t i = r; i < h; ++i){
			dW[r][i] = 1;         //f(w+1, h, r) = ... + f(w, h, i) + ...
			dW[r+h][i+h] = 1;     //g(w+1, h, r) = ... + g(w, h, i) + ...
		}
	}
	
	int64_t (*W)[2*h] = malloc(4*h*h*sizeof(int64_t));
	mat_powmod(2*h, dW, W, w, p, c);
	free(dW);
	int64_t res = 0;
	for(size_t b = 0; b < h; b += c){
		for(size_t r = b; r < b + c && r < h; ++r){
			res += W[r][h];
		}
		res %= p;
	}
	free(W);
	return res;
}

int64_t F_horiz(int64_t w, int64_t h, int64_t p, int64_t c){
	int64_t ret = f_horiz(w, h, p, c) - f_horiz(w, h - 1, p, c);
	ret %= p;
	if(ret < 0){
		ret += p;
	}
	return ret;
}

int main(){
	int64_t w = 4, h = 2, p = 1000000007;
	int32_t n = 2*h;
	int64_t c = ((1ull<<63) - 1)/(p*p) - 1;
	if(c <= 1){
		fprintf(stderr, "\e[1;31mERROR: modulus is too large, cannot prevent overflow\e[0m\n");
		exit(EXIT_FAILURE);
	}
	
}

