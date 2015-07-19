#include <stdio.h>
#include <math.h>

int main(){
	int s = 1;
	for(double d = 2; d < 10; ++d){
		s += floor(log(10.)/log(10./d));
	}
	printf("%i\n", s);
}

