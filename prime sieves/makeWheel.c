#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* Output:
 * #define WHEEL_SIZE
 * #define ORTH_SIZE
 * #define CHARS_PER_WHEEL
 * 
 * const uint8_t offsets[ORTH_SIZE] = {...};
 * const uintn_t lut_offsets[WHEEL_SIZE] = {...};
 * const uint8_t lut_masks[WHEEL_SIZE] = {...};
 */

#define MINSIZE(n) ((n)<0x100?8:(n)<0x10000?16:(n)<0x100000000?32:64)

int main(int argc, char **argv){
	uint64_t n = argc - 1;
	uint64_t ps[n];
	uint64_t wheelSize = 1, orthSize;
	for(uint64_t i = 0; i < n; ++i){
		ps[i] = atoll(argv[i + 1]);
		wheelSize *= ps[i];
	}
	orthSize = wheelSize - n - 1;
	uint64_t wheel[wheelSize];
	memset(wheel, 0, wheelSize*sizeof(uint64_t));
	wheel[0] = 1;
	for(uint64_t i = 0; i < n; ++i){
		wheel[ps[i]] = 1;
		for(uint64_t m = ps[i]*ps[i]; m < wheelSize; m += ps[i]){
			if(!wheel[m]){
				--orthSize;
				wheel[m] = 1;
			}
		}
	}
	if(orthSize%8){
		printf("printf(\"Invalid ORTH_SIZE %%\"PRIu64\"%"PRIu64": not divisible by 8!\", %"PRIu64");\n", orthSize, orthSize);
		return 1;
	}
	unsigned char masks[wheelSize];
	for(uint64_t i = 0, m = 1, o = 0; i < wheelSize; ++i){
		if(wheel[i]){
			wheel[i] = orthSize/8;
			masks[i] = 0;
		}else{
			wheel[i] = o;
			masks[i] = m;
			m <<= 1;
			if(m == 0x100){
				m = 0x01;
				++o;
			}
		}
	}
	printf("#define WHEEL_SIZE %"PRIu64"\n", wheelSize);
	printf("#define ORTH_SIZE %"PRIu64"\n", orthSize);
	printf("#define CHARS_PER_WHEEL %"PRIu64"\n", orthSize/8);
	puts("");
	printf("const uint8_t offsets[ORTH_SIZE] = {");
	uint64_t s = wheelSize;
	for(uint64_t i = 2, l = 1; i < wheelSize; ++i){
		if(masks[i]){
			printf("%"PRIu64",", l);
			s -= l;
			l = 1;
		}else{
			++l;
		}
	}
	printf("%"PRIu64",", s);
	puts("};");
	printf("const uint%d_t lut_offsets[WHEEL_SIZE] = {", MINSIZE(orthSize));
	for(uint64_t i = 0; i < wheelSize; ++i){
		if(masks[i]){
			printf("[%"PRIu64"]=%"PRIu64",", i, wheel[i]);
		}
	}
	puts("};");
	printf("const uint8_t lut_masks[WHEEL_SIZE] = {");
	for(uint64_t i = 0; i < wheelSize; ++i){
		if(masks[i]){
			printf("[%"PRIu64"]=%#x,", i, masks[i]);
		}
	}
	puts("};");
}

