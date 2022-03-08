#include <stdio.h>

int main(void){
	int w = 0;
	#pragma omp parallel for reduction(+:w)
	for(int a = 0; a < 5; ++a){
		for(int b = 0; b < 10; ++b){
			for(int c = 0; c < 10; ++c){
				for(int d = 0; d < 10; ++d){
					int s = a + b + c + d;
					for(int e = 0; e < 10; ++e){
						for(int f = 0; f < 10; ++f){
							for(int g = 0; g < 10; ++g){
								int h = s - (e + f + g);
								if(h < 0 || h > 9){
									continue;
								}
								for(int j = 0; j < 10; ++j){
									int n = s - (b + f + j);
									if(n < 0 || n > 9){
										continue;
									}
									int m = s - (j + g + d);
									if(m < 0 || m > 9){
										continue;
									}
									int k = s - (f + g + j);
									if(k < 0 || k > 9){
										continue;
									}
									int i = s - (a + e + m);
									if(i < 0 || i > 9){
										continue;
									}
									int o = s - (c + g + k);
									if(o < 0 || o > 9){
										continue;
									}
									int p = s - (a + f + k);
									if(p < 0 || p > 9){
										continue;
									}
									int l = s - (d + h + p);
									if(l < 0 || l > 9){
										continue;
									}
									++w;
								}
							}
						}
					}
				}
			}
		}
	}
	printf("%i\n", w<<1);
}

