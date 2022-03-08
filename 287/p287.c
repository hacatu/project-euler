#include <stdio.h>
#include <inttypes.h>

static inline int outside_circle(int64_t N, int64_t i, int64_t j){
	int64_t R = 1ull << (N - 1);
	i = R - i;
	j = R - j;
	return i*i + j*j > R*R;
}

static inline int has_boundary_tl(int64_t N, int64_t k, int64_t i, int64_t j){
	int64_t w = 1ull << k;
	return outside_circle(N, i, j) && !outside_circle(N, i + w - 1, j + w - 1);
}

static inline int has_boundary_tr(int64_t N, int64_t k, int64_t i, int64_t j){
	int64_t w = 1ull << k;
	return outside_circle(N, i + w - 1, j) && !outside_circle(N, i, j + w - 1);
}

static inline int has_boundary_br(int64_t N, int64_t k, int64_t i, int64_t j){
	int64_t w = 1ull << k;
	return outside_circle(N, i + w - 1, j + w - 1) && !outside_circle(N, i, j);
}

static inline int64_t D(uint64_t N){
	// E_N is the "explicit" length, 4^N + (4^(N+1)-1)/3
	/* D_N = E_N - 7*#{Compressable regions}
	 * D_N = E_N - 7*sum_{k=1}^{N-2}#{compressable 2^k blocks}
	 * D_N = E_N - 7*sum_{k=1}^{N-2}(2^{2N-2k} - #{uncompressable 2^k blocks})
	 * D_N = E_N - 7*(4^N - 4^2)/3 + 7*sum_{k=1}^{N-2}#{uncompressable 2^k blocks}
	 */
	int64_t c = 0;
	int64_t R = 1ull << (N - 1);
	//printf("N=%02"PRId64"\n", N);
	printf("N=%02"PRId64, N);
	for(int64_t k = 1; k <= N - 1; ++k){
		//printf(" k=%"PRId64"\n", k);
		int64_t ck = 0;
		int64_t w = 1ull << k;
		int64_t i, j;
		//puts(" Processing top left quadrant");
		for(i = R - w, j = 0; j < i;){
			//printf("  (%"PRId64", %"PRId64")\n", i, j);
			if(has_boundary_tl(N, k, i, j)){
				//puts("  block contains boundary");
				ck += 2;
			}
			if(i && !outside_circle(N, i - 1, j + w - 1)){
				i -= w;
			}else{
				j += w;
			}
		}
		//printf("  (%"PRId64", %"PRId64")\n", i, j);
		if(has_boundary_tl(N, k, i, j)){
			//puts("  block contains boundary");
			ck += 1;
		}
		//puts(" Processing top right quadrant (bottom left is mirror)");
		for(i = R, j = 0; j < R;){
			//printf("  (%"PRId64", %"PRId64")\n", i, j);
			if(has_boundary_tr(N, k, i, j)){
				//puts("  block contains boundary");
				ck += 2;
			}
			if(i < 2*R - 1 && !outside_circle(N, i + w, j + w - 1)){
				i += w;
			}else{
				j += w;
			}
		}
		//puts(" Processing bottom right quadrant");
		for(i = 2*R - w, j = R; j < i;){
			//printf("  (%"PRId64", %"PRId64")\n", i, j);
			if(has_boundary_br(N, k, i, j)){
				//puts("  block contains boundary");
				ck += 2;
			}
			if(!outside_circle(N, i, j + w)){
				j += w;
			}else{
				i -= w;
			}
		}
		//printf("  (%"PRId64", %"PRId64")\n", i, j);
		if(has_boundary_br(N, k, i, j)){
			//puts("  block contains boundary");
			ck += 1;
		}
		//printf(", 2^%02"PRId64":%4"PRId64, k, ck);
		//printf(" 2^%02"PRId64":%4"PRId64"\n", k, ck);
		c += ck;
	}
	printf(", D_N=%"PRId64"\n", 9 + 7*c);
	return 9 + 7*c;
}

int main(){
	D(24);
}

