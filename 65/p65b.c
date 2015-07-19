#include <stdio.h>
#include <inttypes.h>

#define MAX 100

uint64_t e(uint64_t i){
	if(i%3 == 2){
		return (i + 1)/3<<1;
	}
	if(i){
		return 1;
	}
	return 2;
}

/* n[-2] = 0, d[-2] = 1
 * n[-1] = 1, d[-1] = 0
 * n[i] = a[i]*n[i-1] + n[i-2]
 * d[i] = a[i]*d[i-1] + d[i-2]
 * 
 */

int main(){
	uint64_t n1 = 0, d1 = 1, n = 1, d = 0, temp;
	for(uint64_t i = 0; i <= 10; ++i){
		temp = n;
		n = e(i)*n + n1;
		n1 = temp;
		temp = d;
		d = e(i)*d + d1;
		d1 = temp;
		printf("%" PRIu64 ": %" PRIu64 "/%" PRIu64 "\n", i, n, d);
	}
}

