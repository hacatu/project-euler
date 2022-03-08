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

inline void moveFleas(array<uint256_t, 30*30> &board, array<uint256_t, 30*30> &old_board, int64_t h, int64_t k, int64_t t){
	int64_t i = max(0l, h - t);
	//top -> left top -> right top -> right bottom -> left bottom -> bottom
	//top: max(0, h - t)
	//left top: min(h - t + k, h)
	//right top: min(h - t + 29 - k, h)
	//right bottom: min(h + t - 29 + k, 29)
	//left bottom: min(h + t - k, 29)
	//bottom: min(h + t, 0)
	for(; i <= min(h - t + k, h); ++i){//top -> left top
		for(int64_t j = k - t + h - i; j <= t - h + i + k; ++j){
			moveFleasAt(board, old_board[30*i + j], i, j);
		}
	}
	for(; i <= min(h - t + (29 - k), h); ++i){//left top -> right top
		for(int64_t j = 0; j <= t - h + i + k; ++j){
			moveFleasAt(board, old_board[30*i + j], i, j);
		}
	}
	for(; i <= min(h + t - (29 - k), 29l); ++i){//right top -> right bottom
		for(int64_t j = 0; j <= 29; ++j){
			moveFleasAt(board, old_board[30*i + j], i, j);
		}
	}
	for(; i <= min(h + t - k, 29l); ++i){//right bottom -> left bottom
		for(int64_t j = 0; j <= t - i + h + k; ++j){
			moveFleasAt(board, old_board[30*i + j], i, j);
		}
	}
	for(; i <= min(h + t, 29l); ++i){//left bottom -> bottom
		for(int64_t j = k + i - h - t; j <= k - i + h + t; ++j){
			moveFleasAt(board, old_board[30*i + j], i, j);
		}
	}
}

int main(){
	uint256_t M = 847288609443;
	M *= 847288609443;
	M <<= 100;
	array<uint256_t, 30*30> board_a;
	array<uint256_t, 30*30> board_b;
	array<double, 30*30> productboard_a;
	array<double, 30*30> productboard_b;
	productboard_a.fill(1);
	/*
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
	cout << board_a[0] << '\n' << board_a[30*14 + 14] << endl;
	*/
	for(int64_t h = 0; h <= 13; ++h){
		for(int64_t k = h + 1; k <= 14; ++k){
			board_a.fill(0);
			board_a[30*h + k] = M;
			for(int64_t t = 1; t <= 50; ++t){
				board_b.fill(0);
				moveFleas(board_b, board_a, h, k, t);
				swap(board_a, board_b);
			}
			for(int64_t i = 0; i < 30; ++i){
				for(int64_t j = 0; j < 30; ++j){
					productboard_a[30*i + j] *= (double)(M - board_a[30*i + j])/(double)M;
				}
			}
		}
	}
	for(int64_t i = 0; i < 30; ++i){
		for(int64_t j = 0; j < 30; ++j){
			productboard_b[30*i + j] =
				productboard_a[30*i + j] *
				productboard_a[30*j + i];
		}
	}
	for(int64_t h = 0; h <= 14; ++h){
		board_a.fill(0);
		board_a[30*h + h] = M;
		for(int64_t t = 1; t <= 50; ++t){
			board_b.fill(0);
			moveFleas(board_b, board_a, h, h, t);
			swap(board_a, board_b);
		}
		for(int64_t i = 0; i < 30; ++i){
			for(int64_t j = 0; j < 30; ++j){
				productboard_b[30*i + j] *= (double)(M - board_a[30*i + j])/(double)M;
			}
		}
	}
	for(int64_t i = 0; i < 30; ++i){
		for(int64_t j = 0; j < 30; ++j){
			productboard_a[30*i + j] =
				productboard_b[30*i + j] *
				productboard_b[30*(29 - i) + j] *
				productboard_b[30*i + 29 - j] *
				productboard_b[30*(29 - i) + 29 - j];
		}
	}
	cout << "Expected empty squares using product distribution: " <<
		setprecision(17) << accumulate(productboard_a.begin(), productboard_a.end(), 0.) << '\n';
	cout << "Predicted empty squares using overall Poisson: " << 900/std::numbers::e;
}

