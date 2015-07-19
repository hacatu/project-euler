#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../more prime sieves/factorSieve.h"

#define LAST 12000

typedef struct LengthsSet *LengthsSet;

struct LengthsSet{
	LengthsSet a, b, p;
	int length;
};

struct LengthsSet newLengthsSet(int length, LengthsSet p){
	return (struct LengthsSet){.length = length, .p = p};
}

LengthsSet firstPreorder(LengthsSet s){
	while(1){
		if(s->a){
			s = s->a;
		}else if(s->b){
			s = s->b;
		}else{
			return s;
		}
	}
}

LengthsSet nextPreorder(LengthsSet s){
	if(!s->p){
		return NULL;
	}
	if(s == s->p->a && s->p->b){
		return firstPreorder(s->p->b);
	}
	return s->p;
}

void deleteLengthsSet(LengthsSet s){
	s = firstPreorder(s);
	LengthsSet next;
	while((next = nextPreorder(s))){
		free(s);
		s = next;
	}
	free(s);
}

void add(LengthsSet s, int length){
	if(length < s->length){
		if(s->a){
			return add(s->a, length);
		}
		s->a = malloc(1*sizeof(struct LengthsSet));
		if(s->a){
			*s->a = newLengthsSet(length, s);
		}
	}
	if(length > s->length){
		if(s->b){
			return add(s->b, length);
		}
		s->b = malloc(1*sizeof(struct LengthsSet));
		if(s->b){
			*s->b = newLengthsSet(length, s);
		}
	}
}

void addSums(LengthsSet l, LengthsSet a, LengthsSet b, int ones){
	for(LengthsSet i = firstPreorder(a); i; i = nextPreorder(i)){
		for(LengthsSet j = firstPreorder(b); j; j = nextPreorder(j)){
			add(l, i->length + j->length + ones);
		}
	}
}

PrimeFactors *facts;
//PrimeFactors facts[MAX + 1] = {{{0}}};
/* 1471 is the number of primes below 12321, which does not actually matter
 * 5 is the most distinct prime factors a number below 12321 can have,
 * since 2*3*5*7*11 = 2310 <= 12321 and 2*3*5*7*11*13 = 30030 > 12321
 * 12321 is the highest number that could be the lowest number for 12000,
 * because n - 2*sqrt(n) = the number of 1s
 * n + 2 - 2*sqrt(n) = the number of numbers = 12000
 * 0 = n - 2*sqrt(n) - 11998
 * sqrt(n) = (-(-2) +- sqrt((-2)**2 - 4*(1)*(-11998)))/(2*(1))
 * sqrt(n) = 110.540 = 111 //round up
 * n = 12321
 * 12322 because of 0 indexing
 */
 
 //Array of linked lists of integers, 12322 because of zero indexing
LengthsSet lengthSets[MAX + 1] = {NULL};

int minn[LAST + 1] = {[1 ... LAST] = MAX + 1};

 /* count to the next divisor of n by doing n = p_1**a_1 + p_2**a_2 + ... p_i**a_i,
  * divisor = p_1**b_1 + p_2**b_2 + ... + p_i**b_i.  Start b_1 = b_2 = ... = b_i = 0 and add 1 to b_1
  * until b_1 > a_1, then set b_1 = 0 and add 1 to b_2, just like counting
  */
int count(int *div, PrimeFactors divFact, PrimeFactors fact, int factor){
	if(factor == MAX_FACTORS || fact[factor].factor == 0){//all the factors have been counted
		return 0;
	}
	++divFact[factor].power;//note: divFact is .power = n, .prime = p**n; where p is the actual prime
	/* if the prime factor is being overused, set its power to 0 and increment the next one.
	 */
	if(divFact[factor].power > fact[factor].power){
		divFact[factor].power = 0;//set power to 0
		*div /= divFact[factor].factor;//actually remove the power of the prime that was in divisor
		divFact[factor].factor = 1;//set the power of the prime to prime**0 = 1
		return count(div, divFact, fact, factor + 1);//increment the next one
	}else{
		*div *= fact[factor].factor;//actually add (multiply) the prime to the divisor once more
		divFact[factor].factor *= fact[factor].factor;//multiply the current power of the prime by the prime
		return 1;
	}
}

/* count to the next divisor but skip numbers above sqrt(n) = r
 * TODO: do not even generate divisors above sqrt(n)
 */
int skipCount(int *div, PrimeFactors divFact, PrimeFactors fact, int r){
	while(1){
		if(count(div, divFact, fact, 0) == 0){
			return 0;
		}
		if(*div <= r){
			return 1;
		}
	}
}

void allLengths(int n){
	lengthSets[n] = malloc(1*sizeof(struct LengthsSet));
	if(!lengthSets[n]){
		return;
	}
	*lengthSets[n] = newLengthsSet(1, NULL);
	int div = 1;
	PrimeFactors divFact = {[0 ... MAX_FACTORS - 1] = {.factor = 1, .power = 0}};
	while(skipCount(&div, divFact, facts[n], (int)ceil(sqrt(n)))){
		addSums(lengthSets[n], lengthSets[div], lengthSets[n/div], n - div - n/div);
	}
}

int main(){
	facts = factorSieve(MAX);
	if(!facts){
		return 1;
	}
	puts("Done factorSieving");
	for(int n = 1, left = LAST; left && n <= MAX; ++n){
		allLengths(n);
		for(LengthsSet s = firstPreorder(lengthSets[n]); s; s = nextPreorder(s)){
			if(s->length > LAST){
				continue;
			}
			if(n < minn[s->length]){
				if(minn[s->length] == MAX + 1){
					--left;
				}
				minn[s->length] = n;
			}
		}
	}
	puts("Done calculating allLengths");
	LengthsSet s = malloc(1*sizeof(struct LengthsSet));
	if(!s){
		return 1;
	}
	*s = newLengthsSet(1, NULL);
	for(int i = 1; i <= LAST; ++i){
		add(s, minn[i]);
	}
	for(int i = 1; i <= MAX; ++i){
		if(!lengthSets[i]){
			continue;
		}
		deleteLengthsSet(lengthSets[i]);
	}
	int t = 0;
	for(LengthsSet l = firstPreorder(s); l; l = nextPreorder(l)){
		t += l->length;
	}
	t -= 1;
	deleteLengthsSet(s);
	free(facts);
	printf("%i\n", t);
}

