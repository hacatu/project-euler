//14
uint16_t collatzs[1000000] = {[1] = 1};
uint64_t collatz(uint64_t n){
	if(n >= 1000000){
		return 1 + collatz(n&1 ? 3*n + 1 : n>>1);
	}
	if(collatzs[n]){
		return collatzs[n];
	}
	return collatzs[n] = 1 + collatz(n&1 ? 3*n + 1 : n>>1);
}
//30
vector<int>N{0,1,32,243,1024,3125,7776,16807,32768,59049};
int q(int x){
	return N.size()<x?N[x%10]+q(x/10):N[x];
}
//49
bool rp(int a,int b){
  if(a==b)return true;
  int o=a%10,c=b,t=1;
  a/=10;
  while(c){
    if((c%10)==o)return rp(a,s(b,t));
    c/=10;
    t*=10;
  }
  return false;
}
//74
int8_t chainDepths[10000000] = {[1] = 1, [2] = 1, [145] = 1, [169] = 3, [871] = 2, [872] = 2, [1454] = 3, [40585] = 1, [45361] = 2, [45362] = 2, [363601] = 3};
uint8_t chainDepth(uint32_t n){
	if(chainDepths[n] == 0){//not yet computed
		return chainDepths[n] = 1 + chainDepth(digitFactorials[n]);
	}
	return chainDepths[n];
}
//76
uint64_t partitions[MAX + 1] = {[0] = 1};
uint64_t partition(int n){
	if(n < 0){
		return 0;
	}
	if(!partitions[n]){
		for(int k = 1;; ++k){
			int a = (k&1?1:-1)*(partition(n - ((k*(3*k - 1))>>1)));
			if(!a){
				break;
			}
			partitions[n] += a;
			a = (k&1?1:-1)*(partition(n - ((k*(3*k + 1))>>1)));
			if(!a){
				break;
			}
			partitions[n] += a;
		}
	}
	return partitions[n];
}
#79
def combine(s, a, b):
	if len(a) == 0:
		return [s + b]
	if len(b) == 0:
		return [s + a]
	return combine(s + a[0], a[1:], b) + combine(s + b[0], a, b[1:])
//92
int end(uint64_t n){
	n = sumDigitSquares(n);
	if(ends[n]){
		return ends[n];
	}
	return ends[n] = end(n);
}
//95
int next[] =   {1, 2, 1}, cyclens[] = {0, 0, 0}, depth = -1;
int cyclen(int n){
	int len;
	if(cyclens[n] == 0){
		cyclens[n] = --depth;
		len = cyclen(next[n]);
		if(depth == -1)return -1;
		if(depth >= cyclens[n])return cyclens[n] = len;
		return cyclens[n] = -1;
	}else if(cyclens[n] < -1){
		len = cyclens[n] -depth + 1;
		depth = cyclens[n];
		return len;
	}
	depth = -1;
	return cyclens[n];
}
//96
int backtrackingSolve(int *board, int *left){
	int thisBoard[81], thisLeft, pivot = minimumPossibilitiesCell(board);
	for(int i = 1; i <= 9; ++i){
		if(board[pivot]&digitMask[i]){
			memcpy(thisBoard, board, 9*9*sizeof(int));
			thisLeft = *left - 1;
			thisBoard[pivot] = digitMask[i];
			if(!eliminateNeighbors(thisBoard, pivot, &thisLeft)){
				continue;
			}
			if(!thisLeft){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
			if(!repeatedSelect(thisBoard, &thisLeft)){
				continue;
			}
			if(!thisLeft){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
			if(backtrackingSolve(thisBoard, &thisLeft)){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
		}
	}
	return 0;
}
//105
int sumsDiffR(int s, int n){
	if(n < 0){
		if(sums[s]){
			return 0;
		}
		sums[s] = 1;
		return 1;
	}
	return sumsDiffR(s, n - 1) && sumsDiffR(s + set[n], n - 1);
}
//111
uint64_t sumPrimesnd(int l, int n, int d, uint64_t b, uint64_t p){
	if(n < 0){//too many digits have been set to d in the invocation of sumPrimesnd that called this, return 0
		return 0;
	}
	if(n == l){//every remaining digit must be d, so make it so and return (this is the real base case)
		if(!d && l){//d == 0 cannot be the leading digit
			return 0;
		}
		b += d*repdigits[l]*p;
		return isPrime(b)?b:0;
	}
	uint64_t s = 0;
	if(p == 1){//special case, first (one's) digit.  Can be 1, 3, 7, or 9.  2 and 5 would work for 1 digit numbers, but those are never generated by this program
		for(int i = 0; i < ONES_DIGITS_C; ++i){
			b = ONES_DIGITS[i];
			s += sumPrimesnd(l - 1, (b == d)?n - 1:n, d, b, p*10);//Ill advised ternary operator
		}
		return s;
	}
	for(int i = (l == 1)?1:0; i < 10; ++i){
		s += sumPrimesnd(l - 1, (i == d)?n - 1:n, d, i*p + b, p*10);
	}
	return s;
}
