#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

typedef struct{//(rc*sqrt(root)+num)/denom
	int rc;
	int root;
	int num;
	int denom;
} qdr;

int gcd(int a, int b){
	if(a < 0){
		a = -a;
	}
	if(b < 0){
		b = -b;
	}
	if(a < b){
		int temp = a;
		a = b;
		b = temp;
	}
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

//TODO: make calculation more exact
int coef(qdr q){
	return (int)floor((q.rc*sqrt(q.root) + q.num)/q.denom);
}

/* iterate returns the next a_i value, starting at a_0.
 * it also updates q to the next partial fraction
 * //values
 * c: a_i //calculated by coef()
 * s: q.rc
 * r: q.root
 * n: q.num
 * d: q.denom
 * //algorithm
 * q: (s*sqrt(r)+n)/d
 *     //subtract a_i (c)
 *     n = n-c*d
 * q: (s*sqrt(r)+n)/d
 * q: d/(s*sqrt(r)+n) //note q skips this right to the next expression
 *     //multiply top and bottom by conjugate of bottom
 *     temp = d;
 *     d = s*s*r-n*n
 *     s = s*temp
 *     n = -n*temp
 *     //q is now (in terms of the old values): (d*s*sqrt(r)-d*n)/(s*s*r-n*n)
 * q: (s*sqrt(r)+n)/d
 * sorry this is so hard to understand.
 * try running it with q.root = 23 and seeing how it compares to the example.
 */
int iterate(qdr *q){
	int c = coef(*q);
	int d;
	q->num -= c*q->denom;
	d = q->denom;
	q->denom = q->rc*q->rc*q->root - q->num*q->num;
	q->num *= -d;
	q->rc *= d;
	d = gcd(gcd(q->rc, q->num), q->denom);
	if(d != 1){
		q->rc /= d;
		q->num /= d;
		q->denom /= d;
	}
	//printf("c:%i\n", c);
	return c;
}

int orderQdr(qdr a, qdr b){
	return memcmp(&a, &b, sizeof(qdr));
}

void printQdr(qdr q){
	printf("(%i\u221A%i + %i)/%i\n", q.rc, q.root, q.num, q.denom);
}

int main(){
	int odd = 0;
	qdr q, s;
	for(int i = 2, r, c; i < 10000; ++i){
		r = floor(sqrt(i));
		if(i == r*r){
			continue;
		}
		c = 0;
		q = (qdr){.rc = 1, .root = i, .num = 0, .denom = 1};
		//printQdr(q);
		iterate(&q);
		s = q;
		//printQdr(q);
		do{
			++c;
			iterate(&q);
			//printQdr(q);
		}while(orderQdr(q, s));//while q != s
		if(c & 1){
			++odd;
		}
	}
	printf("%i\n", odd);
}

