#include <stdio.h>
#include <string.h>
#include <inttypes.h>

char digits[10];

char isUsed(int n){
	return (n < 1 || n > 9 || digits[n]);
}

/* 10
 *    \
 *      a     f
 *    /   \  /
 *  e       b
 * / \     /
 *i   d --c -- g
 *     \
 *      h
 * i = k - a - e
 * h = k - d - e
 * g = k - c - d
 * f = k - b - c
 * b = k - 10 - a
 * variables: k, a, c, d, e
 * k: 13 , 14 , 15     , 16     , 17         , 18
 * a: 1 2, 1 3, 1 2 3 4, 1 2 4 5, 1 2 3 4 5 6, 1 2 3 5 6 7
 * b: 2 1, 3 1, 4 3 2 1, 5 4 2 1, 6 5 4 3 2 1, 7 6 5 3 2 1
 */

int main(){
	for(int k = 13, i; k <= 18; ++k){
		printf("k: %i\n", k);
		memset(digits, 0, 10*sizeof(char));
		for(int a = 1, b; a < k - 10; ++a){
			digits[a] = 1;
			b = k - a - 10;
			if(isUsed(b)){
				digits[a] = 0;
				continue;
			}
			digits[b] = 1;
			for(int c = 1, f; c < k - b; ++c){
				if(isUsed(c)){
					continue;
				}
				digits[c] = 1;
				f = k - c - b;
				if(isUsed(f)){
					digits[c] = 0;
					continue;
				}
				digits[f] = 1;
				for(int d = 1, g; d < k - c; ++d){
					if(isUsed(d)){
						continue;
					} 
					digits[d] = 1;
					g = k - d - c;
					if(isUsed(g)){
						digits[d] = 0;
						continue;
					}
					digits[g] = 1;
					for(int e = 1, h; e < k - d; ++e){
						if(isUsed(e)){
							continue;
						}
						digits[e] = 1;
						h = k - e - d;
						if(isUsed(h)){
							digits[e] = 0;
							continue;
						}
						digits[h] = 1;
						i = k - a - e;
						if(isUsed(i)){
							digits[h] = 0;
							digits[e] = 0;
							continue;
						}
						{
							int min = g < f ? g : f;
							if(h < min){
								min = h;
							}
							if(i < min){
								min = i;
							}
							if(f == min){
								printf("%i%i%i", f, b, c);
								printf("%i%i%i", g, c, d);
								printf("%i%i%i", h, d, e);
								printf("%i%i%i", i, e, a);
								printf("%i%i%i", 10, a, b);
							}
							if(g == min){
								printf("%i%i%i", g, c, d);
								printf("%i%i%i", h, d, e);
								printf("%i%i%i", i, e, a);
								printf("%i%i%i", 10, a, b);
								printf("%i%i%i", f, b, c);
							}
							if(h == min){
								printf("%i%i%i", h, d, e);
								printf("%i%i%i", i, e, a);
								printf("%i%i%i", 10, a, b);
								printf("%i%i%i", f, b, c);
								printf("%i%i%i", g, c, d);
							}if(i == min){
								printf("%i%i%i", i, e, a);
								printf("%i%i%i", 10, a, b);
								printf("%i%i%i", f, b, c);
								printf("%i%i%i", g, c, d);
								printf("%i%i%i", h, d, e);
							}
							puts("");
						}
						digits[h] = 0; digits[e] = 0;
					}
					digits[g] = 0; digits[d] = 0;
				}
				digits[f] = 0; digits[c] = 0;
			}
			digits[b] = 0; digits[a] = 0;
		}
	}
}

