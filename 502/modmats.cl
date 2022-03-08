__kernel void mat_assign(const int n, const __global long *A, __global long *B){
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	B[i*n + j] = A[i*n + j];
}

__kernel void mat_eye(const int n, __global long *A){
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	A[i*n + j] = (i == j);
}

__kernel void mat_mod_mul(const int n, const __global long *A, const __global long *B, __global long *C, const long p, const int c){
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	long cij = 0;
	for(int b = 0; b < n; b += c){
		for(int k = b; k < b + c && k < n; ++k){
			cij += A[i*n + k]*B[k*n + j];
		}
		cij %= p;
	}
	C[i*n + j] = cij;
}

//binary exponentiation algorithm.  The remainder calculations are in the mat_mod_mul function
__kernel void mat_mod_pow_inplace(const int n, __global long *_A, __global long *_S, __global long *_B, long e, const long p, const int c){
	__global long *A = _A, *S = _S, *B = _B, *T;
	mat_assign(n, A, S);
	if(n&1){
		mat_assign(n, A, B);
	}else{
		mat_eye(n, B);
	}
	while(e >>= 1){
		barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
		mat_mod_mul(n, S, S, A, p, c);
		T = A;//A was our dead variable but now S is so exchange their names
		A = S;
		S = T;//now A is dead again
		if(e&1){
			barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
			mat_mod_mul(n, B, S, A, p, c);
			T = A;//A was our dead variable but now B is so exchange their names
			A = B;
			B = T;//now A is dead again
		}
	}
	if(B != _B){//it is possible to figure out which of the 3 buffers will have the output a priori
		mat_assign(n, B, _B);//but it would just be a copy then vs a copy now *shrug*
	}
}

