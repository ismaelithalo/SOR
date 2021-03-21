#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10000
#define N_THREADS 4

float **C;
float **A;
float **B;

void *multiply(void *tid) 
{ 
	printf("thread %ld\n",(long) tid);
	int i, j, k;
	long t = (long) tid;
	
	/* 
	 *
	/codigo de multiplicacao concorrente aqui
	*
	*/

	pthread_exit(NULL); //thread termina
} 

int main(int argc, char **argv) 
{ 
	int i, j; 
	pthread_t threads[N_THREADS];
	int status;

	//alocacao dinamica das matrizes A, B e C
	A = (float **) malloc(N*sizeof(float*));
	for(i = 0 ; i < N ; i ++)
		A[i] = (float*) malloc(N*sizeof(float));

	B = (float **) malloc(N*sizeof(float*));
	for(i = 0 ; i < N ; i ++)
		B[i] = (float*) malloc(N*sizeof(float));

  	C = (float **) malloc(N*sizeof(float*));
	for(i = 0 ; i < N ; i ++)
		C[i] = (float*) malloc(N*sizeof(float));

	//inicializacao das matrizes
	int count = 1;
	for (i = 0; i < N; i++) 
	{ 
		for (j = 0; j < N; j++) 
		{
			A[i][j] = count;
			B[i][j] = count;
			C[i][j] = 0;
			count++;
		}
	} 

	//criacao das threads
	for(i = 0 ; i < N_THREADS ; i ++) 
	{
		status = pthread_create(&threads[i], NULL, multiply, (void*) i);
		if(status != 0)
		{
			printf("Cannot create thread\n");
			exit(1);
		}
	}

	//aguarda as threads terminarem
	for(i = 0 ; i < N_THREADS ; i ++) 
	{
		pthread_join(threads[i],NULL);
	}

	
	//imprime a matriz caso ela tenha tamanho menor que 5x5
	if(N < 5)
	{
		for (i = 0; i < N; i++) 
		{ 
			for (j = 0; j < N; j++) 
			{
				printf("%.2f ",C[i][j]);
			}
			printf("\n"); 
		} 
	}
	return 0; 
} 
