#include <stdio.h>
#include <inttypes.h>

#define W 43
#define H 47

uint64_t evenRects(uint64_t w, uint64_t h, uint64_t n, uint64_t m){
	return (h + 1 - ((n + 1)/2) - ((m + 1)/2))*(w + 1 - ((n + m + 1)/2));
}

uint64_t oddRects(uint64_t w, uint64_t h, uint64_t n, uint64_t m){
	return (h - (n/2) - (m/2))*(w - ((n + m)/2));
}

uint64_t diagRects(uint64_t w, uint64_t h, uint64_t n, uint64_t m){
	return evenRects(w, h, n, m) + oddRects(w, h, n, m);
}

uint64_t countDiag(uint64_t w, uint64_t h){
	uint64_t c = 0;
	for(uint64_t n = 1; n <= 2*w - 1; ++n){
		for(uint64_t m = 1; m <= 2*w - n; ++m){
			c += evenRects(w, h, n, m) + oddRects(w, h, n, m);
		}
	}
	return c;
}

uint64_t countOrth(uint64_t w, uint64_t h){
	return w*(w + 1)*h*(h + 1)/4;
}

uint64_t countRects(uint64_t w, uint64_t h){
	return countOrth(w, h) + countDiag(w, h);
}

int main(void){
	uint64_t c = 0;
	#pragma omp parallel for reduction(+:c)
	for(uint64_t w = 1; w <= W; ++w){
		for(uint64_t h = 1; h < w; ++h){
			c += 2*countRects(h, w);
		}
		c += countRects(w, w);
		for(uint64_t h = W + 1; h <= H; ++h){
			c += countRects(w, h);
		}
	}
	printf("%"PRIu64"\n", c);
}

