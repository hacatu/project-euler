#include <iostream>
#include <math.h>
using namespace std;
int main(){
	int s[10000],d=0,e,r;
	for(int i=1;i<10001;i++){
		s[i-1]=1;
		r=sqrt(i);
		for(int j=2;j<=r;j++){
			if(i%j==0){
				s[i-1]+=j;
				if(j!=r)s[i-1]+=i/j;
			}
		}
	}
	for(int i=1;i<10001;i++){
		e=s[i-1];
		if(e<10001&&e!=i&&s[e-1]==i)d+=i;
	}
	cout<<d;
}
