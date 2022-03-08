#include <map>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

const size_t LEN = 44;
const uint256_t is[LEN] = {4,5,6,7,8,9,10,11,12,14,15,16,18,20,21,22,24,25,27,28,30,32,33,35,36,40,42,44,45,48,49,50,54,55,56,60,63,64,66,70,72,75,77,80};

std::map<uint256_t, uint64_t> sums, nsums;

uint256_t iss[LEN], rss[LEN], target, denom;

uint256_t gcd(uint256_t a, uint256_t b){
	while(1){
		if(a){
			b %= a;
		}else{
			return b;
		}
		if(b){
			a %= b;
		}else{
			return a;
		}
	}
}

uint256_t lcm(uint256_t a, uint256_t b){
	return a*b/gcd(a, b);
}

int main(){
	sums[0] += 1;
	
	denom = 1;
	
	for(auto i : is){
		denom = lcm(denom, i*i);
	}
	
	std::cout << "denom: " << denom << "\n";
	
	target = 0;
	
	for(size_t i = 0; i < LEN; ++i){
		target += (iss[i] = denom/(is[i]*is[i]));
	}
	
	rss[LEN - 1] = iss[LEN - 1];
	for(size_t i = LEN - 1; i-- > 0;){
		rss[i] = rss[i + 1] + iss[i];
	}
	
	target -= 5*denom/36;
	
	std::cout << "target:  " << 5*denom/36 << "\n";
	std::cout << "~target: " << target << "\n";
	
	for(size_t i = 0; i < LEN; ++i){
		std::cout << i << ":" << sums.size() << "\n";
		if(target > rss[i]){
			sums.erase(sums.begin(), --sums.lower_bound(target - rss[i]));
		}
		sums.erase(sums.upper_bound(target - iss[i]), sums.end());
		for(auto s = sums.rbegin(); s != sums.rend(); ++s){
			sums[s->first + iss[i]] += s->second;
		}
	}
	std::cout << sums[target];
}

