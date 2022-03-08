#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Ulam{
	private:
	vector<uint64_t> sequence;
	uint64_t a, l, d;
	map<uint64_t, uint8_t> cand;
	public:
	Ulam(uint64_t k) : sequence{2, (k << 1) + 1}, a(0), l(0), d(0) {
		printf("[0]=2\n[1]=%" PRIu64 "\n", (k << 1) + 1);
	}
	uint64_t operator[](uint64_t n){
		if(n < sequence.size()){
			return n;
		}
		if(!l){
			find_cycle();
		}
		return (n - a)/l*d + sequence[(n - a)%l + a];
	}
	private:
	void find_cycle(){
		cand.insert(pair<uint64_t, uint8_t>(sequence[0] + sequence[1], 1));
		for(int64_t k = 1;; ++k){
			find_upto(k);
			for(int64_t n = 1; n <= (k - 1) >> 1; ++n){
				for(int64_t i = -1; i < 9; ++i){
					find_upto(k + n*(i + 1));
					for(int64_t j = 0; j < n; ++j){
						if(sequence[k - (n << 1) + 1 + j] - sequence[k - (n << 1) + j] != sequence[k + n*i + 1 + j] - sequence[k + n*i + j]){
							goto CONTINUE_L;
						}
					}
				}
				a = k - (n << 1);
				l = n;
				d = sequence[k - n] - a;
				CONTINUE_L:;
			}
		}
		cand.clear();
	}
	void find_upto(uint64_t k){
		while(sequence.size() < k){
			auto next = cand.begin();
			while(next->second & 2){
				++next;
			}
			uint64_t c = next++->first;
			cand.erase(cand.begin(), next);
			for(uint64_t u : sequence){
				uint64_t n = c + u;
				auto hint = cand.lower_bound(n);
				if(hint == cand.end() || hint->first != n){
					cand.insert(hint, pair<uint64_t, uint8_t>(n, 1));
				}else{
					hint->second |= 2;
				}
			}
			printf("[%zu]=%" PRIu64 "\n", sequence.size(), c);
			sequence.push_back(c);
		}
	}
};

int main(){
	Ulam ulam(2);
	ulam[2];
}

