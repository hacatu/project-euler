#include <stdio.h>

int maxP(int *d, int l, int m){
	int p = 1;
	int i = 0;
	for(int e = i + m; i < e && i < l; ++i){
		if(!d[i]){
			e = i + m + 1;
			p = 1;
			continue;
		}
		p *= d[i];
	}
	int max = p;
	for(; i < l; ++i){
		if(!d[i]){
			for(int e = i + m; i < e && i < l; ++i){
				if(!d[i]){
					e = i + m + 1;
					p = 1;
					continue;
				}
				p *= d[i];
			}
			if(p > max){
				max = p;
			}
			continue;
		}
		p = p/d[i - m]*d[i];
		if(p > max){
			max = p;
		}
		continue;
	}
	return max;
}

int main(void){
	
}

