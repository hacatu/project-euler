#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static const uint64_t K = 100000000000ul;//1 indexed

uint64_t Nn[] = {32, 26, 444, 1628, 5906, 80, 126960, 380882, 2097152};
uint64_t Dn[] = {126, 126, 1778, 6510, 23622, 510, 507842, 1523526, 8388606};

uint64_t u2nk(uint64_t n, uint64_t K){
	uint64_t v = 2*n + 1;
	if(K == 1){
		return 2;
	}else if(K == (v + 7)/2){
		return 2*v + 1;
	}else if(K == 2){
		return v;
	}
	uint64_t N = (K - 3)/Nn[n - 2];
	uint64_t k = (K - 3)%Nn[n - 2];//0 indexed
	//add Dn*N to the nth odd term of u
	//the second even term is 2v+2 for k=(v+7)/2, 1 indexed
	printf("u(2,%"PRIu64")[%"PRIu64"]=u[%"PRIu64"] + %"PRIu64"*%"PRIu64"\n", v, K, k + 1, Dn[n - 2], N);
	k++;//0 indexed but accounting for the first even term
	if(k <= v + 1){
		return v + 2*(k - 1) + Dn[n - 2]*N;
	}else{
		k++;//0 indexed but accounting for the second even term
		uint64_t *uo = malloc(N*sizeof(uint64_t));
		uint64_t j = 0;
		for(; j < v + 1; ++j){
			uo[j] = v + 2*j;
		}
		//2v + 2 + v is the first odd collision
		uint64_t e1 = 2, e2 = 2*v + 2, i1 = v + 2, i2 = 1;
		while(j <= k - 2){
			if(uo[i1] + e1 == uo[i2] + e2){
				i1++, i2++;
			}else if(uo[i1] + e1 < uo[i2] + e2){
				uo[j++] = uo[i1++] + e1;
			}else{
				uo[j++] = uo[i2++] + e2;
			}
		}
		uint64_t ret = uo[k - 2] + Dn[n - 2]*N;
		free(uo);
		return ret;
	}
}

int main(){
	for(uint64_t k = 35; k <= 66; ++k){
		uint64_t val = u2nk(2, k);
		printf("u(2,5)[%"PRIu64"]=%"PRIu64"\n", k, val);
	}
}

