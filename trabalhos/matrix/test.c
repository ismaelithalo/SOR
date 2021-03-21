#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

double **C;
double **A;
double **B;

void multiply() 
{ 
	long i, j, k;

	for(i = 0; i < N; i++)
	{
		for(j=0;j<N;j++)
		{
			for(k = 0;k<N;k++)
			{	
				C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
			}
		}	
	}
} 

int main(int argc, char **argv) 
{ 
	int i, j; 
	int status;

	//alocacao dinamica das matrizes A, B e C
	A = (double **) malloc(N*sizeof(double*));
	for(i = 0 ; i < N ; i ++)
		A[i] = (double*) malloc(N*sizeof(double));

	B = (double **) malloc(N*sizeof(double*));
	for(i = 0 ; i < N ; i ++)
		B[i] = (double*) malloc(N*sizeof(double));

  	C = (double **) malloc(N*sizeof(double*));
	for(i = 0 ; i < N ; i ++)
		C[i] = (double*) malloc(N*sizeof(double));

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
	
	multiply();
	
	//imprime a matriz
	if(1)
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
