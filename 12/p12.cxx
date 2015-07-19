#include <iostream>
#include <math.h>
using namespace std;
long fact(long n){return n?n*fact(n-1):1;}
long tPDivs(long x){
	if(x==1)
		return 0;
	if(x%2==0)
		return 1+tPDivs(x/2);
	float s=sqrt(x);
	for(int i=3;i<=s;i+=2){
		if(x%i==0)
			return 1+tPDivs(x/i);
	}
	return 1;
}
long mPDivs(long x){
	if(x==1)
		return 1;
	int m=0;
	while(x%2==0){
		x/=2;
		m++;
	}
	if(m==0){
		float s=sqrt(x);
		for(int i=3;i<=s;i+=2){
			while(x%i==0){
				x/=i;
				m++;
			}
			if(m!=0)
				break;
		}
	}
	if(m==0)
		return 1;
	else
		return fact(m)*mPDivs(x);
}
int main(){
	long t=1;
	long i=1;
	while(pow(2,tPDivs(t))/mPDivs(t)<500){
		i++;
		t=i*(i-1)/2;
	}
	cout<<t;
}
