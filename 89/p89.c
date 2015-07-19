#include <stdio.h>

int fromRoman(char *buf, int *len){
	int value = 0;
	char *cur = buf;
	while(1){
		switch(*cur){
			case 'M':
				++cur;
				value += 1000;
				break;
			case 'D':
				++cur;
				value += 500;
				break;
			case 'L':
				++cur;
				value += 50;
				break;
			case 'V':
				++cur;
				value += 5;
				break;
			case 'C':
				++cur;
				switch(*cur){
					case 'M':
						++cur;
						value += 900;
						break;
					case 'D':
						++cur;
						value += 400;
						break;
					default:
						value += 100;
						break;
				}
				break;
			case 'X':
				++cur;
				switch(*cur){
					case 'C':
						++cur;
						value += 90;
						break;
					case 'L':
						++cur;
						value += 40;
						break;
					default:
						value += 10;
						break;
				}
				break;
			case 'I':
				++cur;
				switch(*cur){
					case 'X':
						++cur;
						value += 9;
						break;
					case 'V':
						++cur;
						value += 4;
						break;
					default:
						value += 1;
						break;
				}
				break;
			default:
				*len = cur - buf;
				return value;
		}
	}
}

int lenRoman(int n){
	int len = 0;
	if(n >= 1000){//M
		len += n/1000;
		n %= 1000;
	}
	if(n >= 900){//CM
		len += 2;
		n -= 900;
	}else if(n >= 500){//D
		len += 1;
		n -= 500;
	}else if(n >= 400){//CD
		len += 2;
		n -= 400;
	}
	if(n >= 100){//C
		len += n/100;
		n %= 100;
	}
	if(n >= 90){//XC
		len += 2;
		n -= 90;
	}else if(n >= 50){//L
		len += 1;
		n -= 50;
	}else if(n >= 40){//XL
		len += 2;
		n -= 40;
	}
	if(n >= 10){//X
		len += n/10;
		n %= 10;
	}
	if(n >= 9){//XI
		n -= 9;
		len += 2;
	}else if(n >= 5){//V
		n -= 5;
		len += 1;
	}else if(n >= 4){//IX
		n -= 4;
		len += 2;
	}
	len += n;//X
	return len;
}

int main(){
	FILE *file = fopen("roman.txt", "r");
	if(!file){
		return 1;
	}
	int saved = 0;
	{
		char buf[26];//Maximum number is MMMMCMDCCCCXCLXXXXIXVIIII, 25 letters long
		int value, len;
		for(;fscanf(file, "%25s", buf) > 0;){//poor abused for loop
			value = fromRoman(buf, &len);
			saved += len - lenRoman(value);
		}
	}
	printf("%i\n", saved);
	fclose(file);
}

