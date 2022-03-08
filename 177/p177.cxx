#include <iostream>
#include <map>
#include <vector>
#include <cstdint>
#include <math.h>

using namespace std;

static constexpr long double EPSILON = 8e-10;

static double sin_table[181];

struct Triangle{
	uint8_t A, B, C;
	long double p, q;
	
	Triangle(uint8_t A, uint8_t B, uint8_t C) : A(A), B(B), C(C) {
		p = sin_table[A + C]*sin_table[B];
		q = sin_table[C - B]*sin_table[A];
	}
	
	constexpr int _ord(const Triangle &other) const {
		double ord = p*other.q - other.p*q;
		if(ord < -EPSILON){
			return -1;
		}else if(ord > EPSILON){
			return 1;
		}
		return 0;
	}
	
	constexpr bool operator<(const Triangle &other) const {return this->_ord(other) < 0;}
	constexpr bool operator<=(const Triangle &other) const {return this->_ord(other) <= 0;}
	constexpr bool operator>(const Triangle &other) const {return this->_ord(other) > 0;}
	constexpr bool operator>=(const Triangle &other) const {return this->_ord(other) >= 0;}
	constexpr bool operator==(const Triangle &other) const {return this->_ord(other) == 0;}
	constexpr bool operator!=(const Triangle &other) const {return this->_ord(other) != 0;}
};

int main(int argc, char **argv){
	for(uint64_t i = 0; i < 181; ++i){
		sin_table[i] = sinl(M_PI*i/180.);
	}
	map<Triangle, vector<Triangle>> ratio_groups;
	for(uint64_t C = 2; C < 91; ++C){
		//cout << "C: " << C << endl;
		for(uint64_t A = 1; A < 90; ++A){
			for(uint64_t B = 1; B < C; ++B){
				auto T = Triangle(A, B, C);
				ratio_groups[T].push_back(T);
			}
		}
	}
	cout << ratio_groups.size() << " unique ratios (out of " << 89*89*45 << " triples of angles)" << endl;
}

