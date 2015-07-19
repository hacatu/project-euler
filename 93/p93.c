#include <stdio.h>

char results[3025];

void addRights(float a, float b, float c, float d){
	
}

void addSingle(float a, float b, float c, float d){
	addRights(a, b, c, d);
	addRightx(a, b, c, d);
	addQuads(a, b, c, d);
	addLeftx(a, b, c, d);
	addLefts(a, b, c, d);
}

void addDoubles(float c, float d, float a, float b){
	addSingle(c, d, a, b);
	addSingle(c, d, b, a);
}

void addTriples(float d, float a, float b, float c){
	addDoubles(d, a, b, c);
	addDoubles(d, b, c, a);
	addDoubles(d, c, a, b);
}

int countConsec(float a, float b, float c, float d){
	memset(results, 0, 3025*sizeof(char));
	addTriples(a, b, c, d);
	addTriples(b, c, d, a);
	addTriples(c, d, a, b);
	addTriples(d, a, b, c);
	int r = 0;
	while(results[r + 1]){
		++r;
	}
	return r;
}

int main(){
	int m = 28, n, s = 1234;
	for(int d = 4; d < 10; ++d){
		for(int c = 3; c < d; ++c){
			for(int b = 2; b < c; ++b){
				for(int a = 1; a < b; ++a){
					n = countConsec(a, b, c, d);
					if(n > m){
						m = n;
						s = 1000*a + 100*b + 10*c + 1*d;
					}
				}
			}
		}
	}
	prfloatf("%i\n", s);
}

