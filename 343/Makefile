CC := gcc
LD := gcc
AR := ar
#CFLAGS := -Wall -std=c99 -g3 -ftrapv
CFLAGS := -Wall -std=c99 -Ofast -march=native -mtune=native
#LDFLAGS := -g3 -lm -lprimesieve -ftrapv
LDFLAGS := -lm -lprimesieve -Ofast -flto -fuse-ld=gold -march=native -mtune=native

all: test_factorization p343

test_factorization: test_factorization.o factorization.o
	$(CC) $(LDFLAGS) -o $@ $^

p343: p343.o factorization.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf {test_,}factorization{.o,}

