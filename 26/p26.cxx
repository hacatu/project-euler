#include <iostream>
#include <vector>
using namespace std;
int main(){
	int m=0,n=1,c,r;
	unsigned long long i;
	for(int d=3;d<1000;d+=2){
		c=d;
		i=9;
		r=1;
		while(c%5==0){c/=5;cout<<"/5\n";}
		while(i%c){i=10*i+9;r++;cout<<"9:\n";}
		if(i>m){m=i;n=d;}
		cout<<d<<':'<<r<<endl;
	}
	cout<<n;
}
