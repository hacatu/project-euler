#include <gmpxx.h>
#include <map>
#include <iostream>

const int MIN = 2;
const int MAX = 35;

std::map<mpq_class, int> sums;

mpq_class iss[MAX - MIN + 1], zero = mpq_class(0), target = mpq_class(-1/2);

int main(){
	sums[zero] += 1;
	
	for(int i = MIN; i <= MAX; ++i){
		target += (iss[i - MIN] = mpq_class(1)/mpq_class(i)/mpq_class(i));
	}
	
	for(mpq_class i : iss){
		std::cout << i << "\n";
		for(auto s = sums.rbegin(); s != sums.rend(); ++s){
			if(s->first + i > target){
				break;
			}
			sums[s->first + i] += s->second;
		}
	}
	std::cout << sums[target];
}

