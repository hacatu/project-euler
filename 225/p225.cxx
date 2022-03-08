#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

bool isNonDivisor(uint64_t n){
	uint64_t t1 = 1, t2 = 1, t3 = 1, t/*, i = t1*n*n + t2*n + t3*/;
	//vector<bool> visited(n*n*n);
	while(true){
		//visited[i] = true;
		t = (t1 + t2 + t3)%n;
		if(!t){
			return false;
		}
		/*i -= t1*n*n;
		i *= n;
		i += t;*/
		t1 = t2;
		t2 = t3;
		t3 = t;
		if(t3 == 1 && t2 == 1 && t1 == 1){
			return true;
		}
	}
	return true;
}

int main(){
	for(uint64_t n = 3, c = 0;; n += 2){
		if(isNonDivisor(n)){
			if(++c == 124){
				cout << c << ": " << n << endl;
				break;
			}
		}
	}
}

