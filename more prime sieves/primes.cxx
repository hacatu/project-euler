#include "primes.h"
static std::vector<int>Primes::primes(int m,bool two=true){
	std::vector<int>l;
	bool p;
	if(two)l.push_back(2);
	for(int n=3;n<m;n++){
		p=true;
		for(int i=two;i<l.size();i++){
			if(n%i==0){
				p=false;
				break;
			}
		}
		if(p)l.push_back(n);
	}
	return l;
}
