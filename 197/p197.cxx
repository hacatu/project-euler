#include <iostream>
#include <cstdint>
#include <cmath>

//(0.681175845, 1.029461872)

using namespace std;

const int64_t n = 1000000000000ull;

int32_t f_1e9(double x){
	return floor(exp2(-fma(x, x, -30.403243784)));
}

int main(){
	cout.precision(9);
	int32_t lb = -1000000000, ub = f_1e9(0);
	while(1){
		double lb2 = f_1e9(ub*1e-9), ub2 = f_1e9(lb2*1e-9);
		if(lb2 == lb && ub2 == ub){
			break;
		}
		lb = lb2, ub = ub2;
	}
	cout << fixed << lb*1e-9 << " <= f(x) <= " << fixed << ub*1e-9 << endl;
	int32_t u = -1000000000;
	int32_t *idx = new int32_t[ub - lb + 1];
	int64_t i = 0, p = 0;
	while(1){
		u = f_1e9(u*1e-9);
		++i;
		if(idx[u - lb] != 0){
			p = idx[u - lb];
			break;
		}
		idx[u - lb] = i;
	}
	delete[] idx;
	cout << fixed << u*1e-9 << " appears at index " << p << " and again at " << i << endl;
	//1.029461839 appears at index 516 and again at 518
	i += (n - i)/(i - p)*(i - p);
	cout << "resuming at index " << i << " with " << fixed << u*1e-9 << endl;
	for(;i < n; ++i){
		u = f_1e9(u*1e-9);
	}
	cout << "final: " << (u + f_1e9(u*1e-9))*1e-9 << endl;
}

