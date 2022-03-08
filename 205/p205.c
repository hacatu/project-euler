#include <stdio.h>
#include <inttypes.h>

void fill_pdf(uint64_t *pdf, uint64_t n, uint64_t m, uint64_t d){
	if(!n--){
		*pdf += d;
		return;
	}
	for(uint64_t i = 1; i <= m; ++i){
		fill_pdf(pdf + i, n, m, d);
	}
}

void fill_cdf(uint64_t *cdf, uint64_t n, uint64_t m){
	for(uint64_t i = n; i < n*m; ++i){
		cdf[i + 1] += cdf[i];
	}
}

int main(void){
	//9 4 sided dice vs 6 6 sided dice
	//191102976
	uint64_t cdf9_4[37] = {0};
	uint64_t cdf6_6[37] = {0};
	fill_pdf(cdf9_4, 9, 4, 729);
	fill_pdf(cdf6_6, 6, 6, 4096);
	fill_cdf(cdf9_4, 9, 4);
	fill_cdf(cdf6_6, 6, 6);
	uint64_t s = 0;
	for(uint64_t x = 9; x < 37; ++x){
		s += (cdf9_4[x] - cdf9_4[x - 1])*cdf6_6[x - 1];
	}
	printf("%.7Lf\n", (long double)s/36520347436056576.l);
}

