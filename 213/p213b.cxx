#include <iostream>
#include <array>
#include <boost/multiprecision/cpp_int.hpp>
#include <random>
#include <algorithm>
#include <numeric>
#include <numbers>
#include <iomanip>
 
using namespace boost::multiprecision;
using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> U0_12(0, 12);

inline void moveFleasAt(array<uint256_t, 30*30> &board, uint256_t m, int64_t i, int64_t j){
	m /= ((i == 0 || i == 29) ? 1 : 2) + ((j == 0 || j == 29) ? 1 : 2);
	if(i != 0){board[30*(i - 1) + j] += m;}
	if(i != 29){board[30*(i + 1) + j] += m;}
	if(j != 0){board[30*i + j - 1] += m;}
	if(j != 29){board[30*i + j + 1] += m;}
}

int main(){
	uint256_t M = 847288609443;
	M *= 847288609443;
	M <<= 100;
	array<uint256_t, 30*30> board_a;
	array<uint256_t, 30*30> board_b;
	
	board_a.fill(M);
	for(int64_t t = 0; t < 50; ++t){
		for(int64_t i = 0; i < 30; ++i){
			for(int64_t j = 0; j < 30; ++j){
				moveFleasAt(board_b, board_a[30*i + j], i, j);
			}
		}
		board_a.fill(0);
		swap(board_a, board_b);
	}
	double ez = 0.;
	for(uint256_t e : board_a){
		ez += exp(-(double)e/(double)M);
	}
	
	cout << "Predicted empty squares using overall Poisson: " <<
		setprecision(17) << 900/std::numbers::e << '\n';
	cout << "Predicted empty squares using local Poisson: " << ez << '\n';
	cout << "Accepted value (from product distribution): " << "330.72115401441022\n";
}

