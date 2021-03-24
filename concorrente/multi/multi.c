#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define N_THREADS 1

long **C;
long **A;
long **B;

void *multiply(void *tid) 
{ 
	int i, j, k; 
	int t = (int) tid;
	int tam_bloco = N/N_THREADS;

	printf("thread %d\n",t);

	for (i = t*tam_bloco; i < (t+1)*tam_bloco; i++) 
	{ 
		for (j = 0; j < N; j++) 
		{ 
			C[i][j] = 0; 
			for (k = 0; k < N; k++) 
				C[i][j] += A[i][k] * B[k][j]; 
		} 
	} 
	pthread_exit(NULL);
} 

int main(int argc, char **argv) 
{ 
	int i, j; 
	pthread_t threads[N_THREADS];
	int status;

	A = (long **) malloc(N*sizeof(long*));
	for(i = 0 ; i < N ; i ++)
		A[i] = (long*) malloc(N*sizeof(long));

	B = (long **) malloc(N*sizeof(long*));
	for(i = 0 ; i < N ; i ++)
		B[i] = (long*) malloc(N*sizeof(long));

	C = (long **) malloc(N*sizeof(long*));
	for(i = 0 ; i < N ; i ++)
		C[i] = (long*) malloc(N*sizeof(long));

	int count = 0;
	for(i = 0 ; i < N ; i++)
	{
		for(j = 0; j < N ; j++)
		{
			A[i][j] = count;
			B[i][j] = count;
			C[i][j] = 0;
			count++;
		}
	}


	for(i = 0 ; i < N_THREADS ; i ++) 
	{
		status = pthread_create(&threads[i], NULL, multiply, (void*)i);
		if(status != 0)
		{
			printf("Cannot create thread\n");
			exit(1);
		}
	}

	for(i = 0 ; i < N_THREADS ; i ++) 
	{
		pthread_join(threads[i],NULL);
	}


	if(N < 5)
	{
		for (i = 0; i < N; i++) 
		{ 
			for (j = 0; j < N; j++) 
			{
				printf("%05ld ",C[i][j]);
			}
			printf("\n"); 
		} 
	}


	return 0; 
} 
