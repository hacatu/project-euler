#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

vector<int> P={3};

void addPrimes(int);

bool isPrime(int n){//Won't work on even ints since only odds are tested
	if(n<2)return false;
	int r=sqrt(n);
	if(r>P.back())addPrimes(r);
	for(int p:P){
		if(p>r)return true;
	    if(!(n%p))return false;
	}
	return true;
}

void addPrimes(int n){
	for(int i=P.back()+2;i<=n;i+=2){
		if(isPrime(i))P.push_back(i);
	}
}

int main(){
	addPrimes(1000);
	int m=1,x,y,d,n;
	for(auto b=P.rbegin();b!=P.rend();++b){
		if(*b<=m)break;
		for(int a=-999;a<0;a+=2){
			d=a*a-4**b;
			if(!d)if(a<=0&&-a<=2*m)continue;
			//if(d>0)if(a<0&&a>=-m-*b/m)break;
			n=1;
			while(isPrime(n*(a+n)+*b))++n;
			if(n>m){
				m=n;
				x=a;
				y=*b;
			}
		}
	}
	printf("n^2%+in+%i was prime for ne[0,%i].\nab=%i\n",x,y,m,x*y);
}
