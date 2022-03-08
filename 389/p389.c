#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct{
	uint64_t s, m;
	double **table;
} DieDistributions;

DieDistributions dsn_pdf[] = {
	{.s=4, .m=1},
	{.s=6, .m=4},
	{.s=8, .m=4*6},
	{.s=12, .m=4*6*8},
	{.s=20, .m=4*6*8*12},
};

void destroy_dsn_pdf(){
	for(size_t i = 0; i < sizeof(dsn_pdf)/sizeof(*dsn_pdf); ++i){
		if(!dsn_pdf[i].table){
			return;
		}
		for(size_t j = 0; j <= dsn_pdf[i].m; ++j){
			if(!dsn_pdf[i].table[j]){
				free(dsn_pdf[i].table);
				dsn_pdf[i].table = NULL;
				return;
			}
			free(dsn_pdf[i].table[j]);
		}
		free(dsn_pdf[i].table);
		dsn_pdf[i].table = NULL;
	}
}

int init_dsn_pdf(){
	for(size_t i = 0; i < sizeof(dsn_pdf)/sizeof(*dsn_pdf); ++i){
		if(!(dsn_pdf[i].table = calloc(dsn_pdf[i].m + 1, sizeof(double*)))){
			destroy_dsn_pdf();
			return 0;
		}
		dsn_pdf[i].table[0] = malloc(1*sizeof(double));
		dsn_pdf[i].table[0][0] = 1;
		for(size_t k = 1; k <= dsn_pdf[i].m; ++k){
			if(!(dsn_pdf[i].table[k] = calloc((dsn_pdf[i].s*k + 1), sizeof(double)))){
				destroy_dsn_pdf();
				return 0;
			}
			for(size_t j = k, d; j <= dsn_pdf[i].s*k; ++j){
				if(j <= 1 + dsn_pdf[i].s*(k - 1)){
					d = 1;
				}else{
					d = j - dsn_pdf[i].s*(k - 1);
				}
				for(; d <= dsn_pdf[i].s && d <= j; ++d){
					dsn_pdf[i].table[k][j] += dsn_pdf[i].table[k - 1][j - d];
				}
				dsn_pdf[i].table[k][j] /= dsn_pdf[i].s;
			}
		}
	}
	return 1;
}

void build_chain_ptable(uint64_t s, uint64_t supp, double out[static s*supp + 1], const DieDistributions F, const double table[supp + 1]){
	uint64_t co_supp = supp*s;
	memset(out, 0, (co_supp + 1)*sizeof(double));
	for(uint64_t x = 1; x <= supp; ++x){
		for(uint64_t f = 1; f <= x*s; ++f){
			out[f] += F.table[x][f]*table[x];
		}
	}
}

double variance(uint64_t supp, const double table[static supp + 1]){
	double u2 = 0, u1 = 0;
	for(uint64_t i = 1; i <= supp; ++i){
		u2 += i*i*table[i];
		u1 += i*table[i];
	}
	return u2 - u1*u1;
}

int main(){
	double *table_a = malloc(2*(4*6*8*12*20 + 1)*sizeof(double));
	if(!table_a){
		printf("Could not allocate memory.\n");
		exit(1);
	}
	if(!init_dsn_pdf()){
		free(table_a);
		printf("Could not allocate memory.\n");
		exit(1);
	}
	double *table_b = table_a + 4*6*8*12*20 + 1;
	memcpy(table_a, dsn_pdf[0].table[1], (4 + 1)*sizeof(double));
	build_chain_ptable(6, 4, table_b, dsn_pdf[1], table_a);
	build_chain_ptable(8, 4*6, table_a, dsn_pdf[2], table_b);
	build_chain_ptable(12, 4*6*8, table_b, dsn_pdf[3], table_a);
	build_chain_ptable(20, 4*6*8*12, table_a, dsn_pdf[4], table_b);
	double v = variance(4*6*8*12*20, table_a);
	free(table_a);
	destroy_dsn_pdf();
	printf("%f\n", v);
}

