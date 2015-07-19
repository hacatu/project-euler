#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
int main(){
	vector<int>P;
	int s=2,r;
	bool p=true;
	for(int i=3;i<2000000;i+=2){
		p=true;
		r=sqrt(i);
		for(int j=0;j<P.size()&&P[j]<=r;j++){
			if(i%P[j]==0){
				p=false;
				break;
			}
		}
		if(p){
			P.push_back(i);
			s+=i;
		}
	}
	cout<<s;
}

