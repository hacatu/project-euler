#include <stdio.h>

int r1s[] =    {1, 11, 111, 1111, 11111, 111111, 1111111, 11111111, 111111111};
int r9s[] =    {9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999};
int powers[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

static int _bsl(int n, int a, int b){
	int m = (a + b)/2;
	int p = powers[m];
	if(m == a){//range has only 2 elements
		return (n < powers[b])?a:b;
	}
	if(n < p){
		return _bsl(n, a, m - 1);
	}
	return _bsl(n, m, b);
}

int bsl(int n){
	return _bsl(n, 0, 8);
}

static int _isInc(int n, int i){
	int s = 0;
	for(int r = r1s[i]; r; r /= 10){
		s += n/r;
		n %= r;
	}
	return s < 10;
}

int isInc(int n){
	return _isInc(n, bsl(n));
}

int isDec(int n){
	int l = bsl(n);
	return _isInc(r9s[l] - n, l);
}

int isBouncy(int n){
	int l = bsl(n);
	return !(_isInc(n, l) || _isInc(r9s[l] - n, l));
}

int main(void){
	for(int n = 101, b = 0; ; ++n){
		if(isBouncy(n)){
			++b;
		}
		if(99*n == 100*b){
			printf("%i\n", n);
			break;
		}
	}
}

