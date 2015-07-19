#include <stdio.h>

#define MAX 9999
#define MIN 1000
#define MIN_STEP 45

char shapes(int n, int f[5][MAX/MIN_STEP]){
	char flags = 0;
	for(int i = 4; i >= 0; --i){
		if(n == f[i][n/MIN_STEP]){
			flags |= 1<<i;
		}
	}
	return flags;
}

int main(){
	int f[5][MAX/MIN_STEP] = {{0}};
	for(int s = 0; s < 5; ++s){
		for(int g = 1, n = 1; n <= MAX; g += s + 1, n += g){
			if(n < MIN){
				continue;
			}
			f[s][n/MIN_STEP] = n;
		}
	}
	char f3, f4, f5, f6, f1, boxcars, threes;
	int i1, i2;
	for(int g = 1, n = 1; n <= MAX; g += 6, n += g){
		if(n < MIN){
			continue;
		}
		i2 = n%100;
		for(int i3 = 10; i3 < 100; ++i3){
			f3 = shapes(i2*100 + i3, f);
			if(!f3){
				continue;
			}
			for(int i4 = 10; i4 < 100; ++i4){
				f4 = shapes(i3*100 + i4, f);
				if(!f4){
					continue;
				}
				for(int i5 = 10; i5 < 100; ++i5){
					f5 = shapes(i4*100 + i5, f);
					if(!f5){
						continue;
					}
					for(int i6 = 10; i6 < 100; ++i6){
						f6 = shapes(i5*100 + i6, f);
						if(!f6){
							continue;
						}
						i1 = n/100;
						f1 = shapes(i6*100 + i1, f);
						if(!f1){
							continue;
						}
						if((f3 | f4 | f5 | f6 | f1) != 037){
							continue;
						}
						boxcars = 0;
						if(f3 & 011){
							f3 ^= 011;
							++boxcars;
						}
						if(f4 & 011){
							f4 ^= 011;
							++boxcars;
						}
						if(f5 & 011){
							f5 ^= 011;
							++boxcars;
						}
						if(f6 & 011){
							f6 ^= 011;
							++boxcars;
						}
						if(f1 & 011){
							f1 ^= 011;
							++boxcars;
						}
						if(boxcars == 1){
							threes = 0;
							if(f3 & 01){
								++threes;
							}
							if(f4 & 01){
								++threes;
							}
							if(f5 & 01){
								++threes;
							}
							if(f6 & 01){
								++threes;
							}
							if(f1 & 01){
								++threes;
							}
							if(!threes){
								continue;
							}
						}
						printf("%i\n", 101*(i1 + i2 + i3 + i4 + i5 + i6));
						return 0;
					}
				}
			}
		}
	}
}

