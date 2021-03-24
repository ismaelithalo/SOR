// Ismael Ithalo - 190030194

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define N_THREADS 4

long **A;                      // Incializando as matrizes em long para caber um maior volume de bytes
long **B;
long **C;

void *multiply(void *tid) 
{ 
	printf("Thread %ld\n",(long) tid);      // Indica no output a quantidade de threads utilizadas 

	long t = (long) tid;
    unsigned int i, j, z;                   // Aletrando o tipo das variáveis para unsigned também por conta do tamanho
	
	long carga = N/N_THREADS;

    for (i = t*carga; i < (t+1)*carga; i++) { 
		for (j = 0; j < N; j++) { 
			C[i][j] = 0; 
			for (z = 0; z < N; z++) {
                C[i][j] = C[i][j] + (A[i][z] * B[z][j]);
            } 
		} 
	} 

	pthread_exit(NULL); //thread termina
} 

// A main já contem todo o código do preenchimento das matrizes e da chamada da multiplicação com as threads 
// logo apenas a multiplicação precisa ser inserida para o programa funcionar a princípio.

int main(int argc, char **argv) 
{ 
	int i, j; 
	pthread_t threads[N_THREADS];
	int status;

	//alocacao dinamica das matrizes A, B e C
	A = (long **) malloc(N*sizeof(long*));              // Como alterei para long o tipo da matriz na declaração, alterei aqui também
	for(i = 0 ; i < N ; i ++)
		A[i] = (long*) malloc(N*sizeof(long));

	B = (long **) malloc(N*sizeof(long*));
	for(i = 0 ; i < N ; i ++)
		B[i] = (long*) malloc(N*sizeof(long));

  	C = (long **) malloc(N*sizeof(long*));
	for(i = 0 ; i < N ; i ++)
		C[i] = (long*) malloc(N*sizeof(long));

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
				printf("%ld ",C[i][j]);
			}
			printf("\n"); 
		} 
	}

	return 0; 
} 

/*

    #Resultados

    Foram feitos uma série de testes e pode-se concluir que o uso de threads é completamente benéfico para
    optimizar o tempo de execução dos programas realizando partes do trabalho paralelamente.

    O resultado mais notável foi ao executar uma multiplicação de 10000 x 10000, durando 240 minutos com 4 threads e
    420 minutos com uma thread apenas.

    Para valores menores em N o resultado também é o esperado, execuções mais rápidas, podém a diferença não é tão discrepante,
    ainda sim houveram reduções de até 70% do tempo.

*/ 
