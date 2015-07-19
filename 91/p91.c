#include <stdio.h>

#define MAX 50

int gcd(int a, int b){
	if(b == 0){
		return a;
	}
	while(1){
		a %= b;
		if(a == 0){
			return b;
		}
		b %= a;
		if(b == 0){
			return a;
		}
	}
}

int main(void){
	int count = 0, dx, dy, d;
	for(int x1 = 1; x1 <= MAX; ++x1){
		for(int y1 = 0; y1 <= MAX; ++y1){
			d = gcd(x1, y1);
			dx = x1/d;
			dy = y1/d;
			for(int x2 = x1, y2 = y1;;){
				x2 -= dy;
				y2 += dx;
				if(x2 < 0 || y2 > MAX){
					break;
				}
				++count;
			}//end for x2, y2
		}//end for y1
	}//end for x1
	count *= 2;//count of right triangles with right angle not at origin
	count += MAX*MAX;//add count of right triangles with right angle at origin
	printf("%i\n", count);
}
