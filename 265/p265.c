#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define GET_BIT(s, i) ((s[(i) >> 6] >> ((i) & 0x3F))&1)
#define SET_BIT(s, i) (s[(i) >> 6] |= 1ull << ((i) & 0x3F))
#define CLEAR_BIT(s, i) (s[(i) >> 6] &= ~(1ull << ((i) & 0x3F)))

int make_first_debruijn(uint64_t n, uint64_t *s){
	uint64_t m = n < 6 ? 1 : (1ull << (n - 6));
	memset(s, 0, m*8);
	uint64_t *l = calloc(m, sizeof(uint64_t));
	if(!l){
		return 0;
	}
	l[0] |= 1;
	for(uint64_t i = n, t = 0; i < (1ull << n); ++i){
		t <<= 1;
		if(!GET_BIT(l, t)){
			continue;
		}
		++t;
		SET_BIT(s, i);
	}
	free(l);
	return 1;
}

void print_debruijn(uint64_t n, const uint64_t *s){
	for(uint64_t i = 0; i < (1ull << n); ++i){
		putc(GET_BIT(s, i) ? '1' : '0', stdout);
	}
}

void rotate_debruijn(uint64_t n, uint64_t *s){
	uint64_t t = 0;
	for(uint64_t i = 0; i < n; ++i){
		t <<= 1;
		t |= GET_BIT(s, i);
	}
	for(uint64_t i = n - 1;;){
		if(!t){//actually rotate
			for(uint64_t o = 0, a, b; o < __builtin_ctzll((1ull << n) - i - 1); ++o){
				a = GET_BIT(s, o);
				for(uint64_t j = o, k; k != o; j = k){
					k = (j + o) & ((1ull << n) - 1);
					b = GET_BIT(s, k);
					if(a){
						SET_BIT(s, k);
					}else{
						CLEAR_BIT(s, k);
					}
					a = b;
				}
			}
			//fix up end of array
			break;
		}
		if(++i == (2 << n)){
			break;
		}	
		t <<= 1;
		t &= ((1ull << n) - 1);
		t |= GET_BIT(s, i);
	}
}

int make_next_debruijn(uint64_t n, uint64_t *s){
	uint64_t k = n < 6 ? 1 : (1ull << (n - 6)), b;
	while(k--){//won't cause termination
		b = __builtin_ffsll(s[k]);//but actually I can't be clever because I have to unwind the set
		if(b){
			break;
		}
	}
	s[k] &= ~b;
	//fix up end of array
	uint64_t j = (k << 6) | __builtin_ctzll(b);
	uint64_t t = 0;
	for(uint64_t i = j - n + 1; i != j; ++i){
		t <<= 1;
		t |= GET_BIT(s, i);
	}
}

int main(){
	
}

