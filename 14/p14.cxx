#include <iostream>
using namespace std;
unsigned int c(unsigned int n){
	if(n==1){
		return 1;
	}else if((n&1)==0){
		return 1+c(n/2);
	}else{
		return 2+c((1+n+(n<<1))>>1);
	}
}
int main(){
	int b=1,l=1,d=1;
	for(int i=500001;i<1000001;i+=2){
		l=c(i);
		if(l>b){
			b=l;
			d=i;
		}
	}
	cout<<d<<' '<<b;
}
