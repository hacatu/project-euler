#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <pthread.h>

#define DIGITS 2
#define CPUS 2
#define MODULUS 1000000000

typedef struct{
	int **a;
	int i;
	pthread_mutex_t lock;
} queue;

queue pool;
pthread_mutex_t reduction_mtx, print_mtx;
_Atomic int generating = 1;

unsigned __int128 factorial(unsigned n){
	unsigned __int128 ret = 1;
	while(n){
		ret *= n--;
	}
	return ret;
}

unsigned __int128 unique_permutations(int *a, int len){
	unsigned __int128 ret = factorial(len);
	for(int i = 0, run = 1; i < len - 1; ++i){
		if(a[i] == a[i + 1]){
			++run;
		}else{
			ret /= factorial(run);
			run = 1;
		}
	}
	return ret;
}

void process_unique_permutations(int *a, int i, int plen, int len, int *sum){
	if(i == plen){
		int m = 0;
		for(int i = 0; i < plen; ++i){
			m = m*10 + a[i];
		}
		pthread_mutex_lock(&print_mtx);
		printf("Head Permutation: %i\n", m);
		pthread_mutex_unlock(&print_mtx);
		if(plen != len){
			unsigned __int128 tail_permutations = unique_permutations(a + plen, len - plen);
			*sum = (*sum + tail_permutations*m)%MODULUS;
		}else{
			*sum = (*sum + m)%MODULUS;
		}
	}
	int j = i;
	if(!j){
		while(j < len && !a[j]){
			++j;
		}
	}
	for(; j < len; ++j){
		if(j < len - 1 && a[j] == a[j + 1]){
			continue;
		}
		int t = a[j];
		memmove(a + i + 1, a + i, (j - i)*sizeof(int));//[0, 1, ..., i, i + 1, ..., j, ..., len - 1]
		a[i] = t;//----------------------------copy a[j] to t, shift all elements from i to j - 1 over by 1, and set a[i] to t
		process_unique_permutations(a, i + 1, plen, len, sum);
		memmove(a + i, a + i + 1, (j - i)*sizeof(int));
		a[j] = t;
	}
}

void process_acsending_partitions(int *a, int i, int len, int min, int left){
	if(!left){
		pthread_mutex_lock(&pool.lock);
		pool.a[pool.i] = malloc((len + 1)*sizeof(int));
		pool.a[pool.i][0] = len;
		memcpy(pool.a[pool.i++] + 1, a, len*sizeof(int));
		pthread_mutex_unlock(&pool.lock);
		return;
	}
	if(i == len){
		return;
	}
	for(int d = min; d < 10 && d*d <= left; ++d){
		a[i] = d;
		process_acsending_partitions(a, i + 1, len, d, left - d*d);
	}
}

void *thread_worker(void *data){
	int sum = 0;
	while(1){
		pthread_mutex_lock(&pool.lock);
		if(!pool.i){
			pthread_mutex_unlock(&pool.lock);
			if(!generating){
				break;
			}
			continue;
		}
		int *a = pool.a[--pool.i];
		pthread_mutex_unlock(&pool.lock);
		int i = *a;
		process_unique_permutations(a + 1, 0, i < 9 ? i : 9, i, &sum);
		free(a);
	}
	pthread_mutex_lock(&reduction_mtx);
	*(int*)data = (*(int*)data + sum)%MODULUS;
	pthread_mutex_unlock(&reduction_mtx);
	return NULL;
}

int main(void){
	int **_pool = malloc(701873*sizeof(int*));//Number of ascending partitions (of length <= 20) of squares up to 9*9*20 into squares up to 81.
	pthread_mutex_t _lock;
	pthread_mutex_init(&_lock, NULL);
	pthread_mutex_init(&reduction_mtx, NULL);
	pthread_mutex_init(&print_mtx, NULL);
	pool = (queue){_pool, 0, _lock};
	int a[DIGITS], sum = 0;
	pthread_t workers[CPUS - 1];
	for(int i = 0; i < CPUS - 1; ++i){
		pthread_create(workers + i, NULL, thread_worker, (void*)&sum);
	}
	for(int i = 1; i*i < 81*DIGITS + 1; ++i){
		pthread_mutex_lock(&print_mtx);
		printf("Generating: %i\n", i);
		pthread_mutex_unlock(&print_mtx);
		process_acsending_partitions(a, 0, DIGITS, 0, i*i);
	}
	generating = 0;
	thread_worker((void*)&sum);
	for(int i = 0; i < CPUS - 1; ++i){
		pthread_join(workers[i], NULL);
	}
	free(_pool);
	pthread_mutex_destroy(&_lock);
	pthread_mutex_destroy(&reduction_mtx);
	pthread_mutex_destroy(&print_mtx);
	printf("%i\n", sum);
}

