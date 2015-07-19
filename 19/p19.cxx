#include <iostream>
int main(){
	int s=0,d=2,M[]={3,0,3,2,3,2,3,3,2,3,2,3};
	for(int y=1901;y<2001;y++){
		for(int m=1;m<13;m++){
			if(d==0)s++;
			d+=M[m];
			if(m==2&&!y%4)d++;
			d%=7;
		}
	}
	std::cout<<s;
}
