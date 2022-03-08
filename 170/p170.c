#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <alloca.h>
#include <assert.h>

const uint64_t ten[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

typedef void (*array_fn_t)(void*, size_t);
typedef struct{
	int digitMask;
	uint64_t t;
	uint64_t product;
	uint64_t key;
} product_t;

product_t *products[9][9][2];//digit multiplier, other multiplier length, result length same or greater by one, index (lookup zero terminated)
size_t products_lens[9][9][2] = {0};//derpy global
uint64_t digit;//derpy global

uint64_t permutations(uint64_t n, uint64_t c){
	uint64_t p = 1;
	while(c--){
		p *= n--;
	}
	return p;
}

void forall_permutations_rec(array_fn_t f, void *a, void *p, size_t i, size_t p_l, size_t a_l, size_t size){
	if(i == p_l){
		f(p, p_l);
		return;
	}
	void *tmp = alloca(size);
	for(size_t j = 0; j < a_l; ++j){
		if(j != a_l - 1){
			memcpy(tmp, a + j*size, size);
			memcpy(a + j*size, a + (a_l - 1)*size, size);
			memcpy(a + (a_l - 1)*size, tmp, size);
			memcpy(p + i*size, tmp, size);
		}
		forall_permutations_rec(f, a, p, i + 1, p_l, a_l - 1, size);
		if(j != a_l - 1){
			memcpy(tmp, a + j*size, size);
			memcpy(a + j*size, a + (a_l - 1)*size, size);
			memcpy(a + (a_l - 1)*size, tmp, size);
		}
	}
}

void forall_permutations(array_fn_t f, void *a, size_t p_l, size_t a_l, size_t size){
	void *p = alloca(p_l*size);
	forall_permutations_rec(f, a, p, 0, p_l, a_l, size);
}

void multiplySingle(int *a, size_t l){
	uint64_t other = 0, product;
	if(!a[0]){
		return;
	}
	for(int i = 0; i < l; ++i){
		other = other*10 + a[i];
	}
	product = digit*other;
	int inMask = 0, outMask = 0, digitMask;
	for(int i = 0; i < l; ++i){
		inMask |= 1 << a[i];
	}
	other = product;
	while(other){
		if(digit != 1 && (outMask & (1 << (other%10)))){
			return;
		}
		outMask |= 1 << (other%10);
		other /= 10;
	}
	digitMask = (outMask << 10) + inMask;
	other = __builtin_popcount(outMask) - l;
	if(l == 9 && other){
		return;
	}
	assert(products_lens[digit - 1][l - 1][other] < permutations(10, l));
	products[digit - 1][l - 1][other][products_lens[digit - 1][l - 1][other]++] =
		(product_t){.digitMask = digitMask, .product = product, .key = product*ten[10 - l - other], .t = ten[l + other]};
}

void __attribute__((constructor)) multiplyEverything(void){
	for(size_t i = 0; i < 9; ++i){
		for(size_t j = 0; j < 9; ++j){
			for(size_t k = 0; k < 2; ++k){
				products[i][j][k] = calloc(permutations(10, j + 1) + 1, sizeof(product_t));
			}
		}
	}
	int digits[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(digit = 1; digit < 10; ++digit){
		printf("%i\n", (int)digit);
		digits[digit] = 9;
		digits[9] = digit;
		for(int length = 1; length < 10; ++length){
			forall_permutations((array_fn_t)multiplySingle, digits, length, 10, sizeof(int));
		}
		digits[digit] = digit;
		digits[9] = 9;
	}
	for(size_t i = 0; i < 9; ++i){
		for(size_t j = 0; j < 9; ++j){
			for(size_t k = 0; k < 2; ++k){
				products[i][j][k] = realloc(products[i][j][k], (products_lens[i][j][k] + 1)*sizeof(product_t));
			}
		}
	}
}

void __attribute__((destructor)) freeEverything(void){
	for(size_t i = 0; i < 9; ++i){
		for(size_t j = 0; j < 9; ++j){
			for(size_t k = 0; k < 2; ++k){
				free(products[i][j][k]);
			}
		}
	}
}

int compare_products_by_key(product_t *a, product_t *b){//sort descending
	if(a->key < b->key){
		return 1;
	}else if(a->key > b->key){
		return -1;
	}else{
		return 0;
	}
}

uint64_t max_order_val(size_t num_chunks, product_t *chunks){
	qsort(chunks, num_chunks, sizeof(product_t), (__compar_fn_t)compare_products_by_key);
	uint64_t ret = 0;
	for(int i = 0; i < num_chunks; ++i){
		ret = ret*chunks[i].t + chunks[i].product;
	}
	return ret;
}

uint64_t max_solution_rec(uint64_t digit, size_t num_chunks, product_t *chunks, int digitMask, size_t length, size_t max_len, int k, uint64_t max){
	if(!length){
		uint64_t submax = max_order_val(num_chunks, chunks);
		printf("%"PRIu64"\n", submax);
		return submax > max ? submax : max;
	}
	for(int len = 1; len <= length && len <= max_len; ++len){
		printf(" %i\n", len);
		for(int k2 = len == length && !k; k2 < 2 - k; ++k){
			for(int i = 0;; ++i){
				product_t p = products[digit - 1][len - 1][k2][i];
				int newMask = p.digitMask;
				if(!newMask){
					break;
				}
				if(digitMask & newMask){
					continue;
				}
				digitMask |= newMask;
				chunks[num_chunks] = p;
				max = max_solution_rec(digit, num_chunks + 1, chunks, digitMask, length - len, len, k | k2, max);
			}
		}
	}
	return max;
}

uint64_t max_solution(void){
	uint64_t max = 0;
	product_t chunks[10] = {0};
	for(uint64_t digit = 1; digit < 10; ++digit){
		printf("%i\n", (int)digit);
		chunks[0] = (product_t){.digitMask = 1 << digit, .product = digit, .key = digit*1000000000, .t = 10};
		max = max_solution_rec(digit, 1, chunks, 1 << digit, 9, 9, 0, max);
	}
	return max;
}

void print_int_tuple(int *a, size_t l){
	printf("(");
	for(size_t i = 0; i < l - 1; ++i){
		printf("%i, ", a[i]);
	}
	printf("%i)\n", a[l - 1]);
}

int count_calls(int *a, size_t l){
	static int calls;
	if(!a){
		return calls = 0;
	}
	return calls++;
}

int main(void){
	printf("%"PRIu64"\n", max_solution());
	/*
	int digits[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(int n = 0; n < 10; ++n){
		for(int c = 0; c <= n; ++c){
			forall_permutations((array_fn_t)count_calls, digits, c, n, sizeof(int));
			printf("P(%1i, %1i): counted = %6i calculated = %6i\n", n, c, count_calls(digits, c), (int)permutations(n, c));
			count_calls(NULL, 0);
		}
	}
	*/
}

