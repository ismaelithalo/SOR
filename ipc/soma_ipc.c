#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid;
    key_t k;
    int shmid;
    int n_workers;
    int blocksize; 

    float *A;
    float *B;
    float *C;

    time_t t1;
    time_t t2;

    if(argc != 3)
    {
      fprintf(stderr,"usage: ./soma_ipc <size_of_vectors> <n_workers>\n");
      return 1;
    }

    unsigned long N = atoi(argv[1]);

    A = (float*) malloc(N*sizeof(float));
    B = (float*) malloc(N*sizeof(float));

    for(int i = 0 ; i < N ; i++)
    {
      A[i] = 1.0f;
      B[i] = 2.0f;
    } 

    n_workers = atoi(argv[2]);
    blocksize = N/n_workers;

    printf("size_of_vectors=%lu\n",N);
    printf("n_workers=%d\n",n_workers);

    k = ftok("/mnt/disco1b/unb/sor/ipc/soma_ipc.c",'R');
    shmid = shmget(k, N*sizeof(float), 0644|IPC_CREAT);

    C = shmat(shmid, (void*)0, 0); //pai associa-se a regiao compartilhada
    if(C == (float *)(-1))
        fprintf(stderr,"erro, nao foi possivel associar (shmat)\n");

    t1 = time(NULL);
    for(int i = 0 ; i < n_workers ; i++)
    {
      pid = fork();
      if(pid < 0)
      {
        fprintf(stderr,"erro, nao eh possivel criar processo filho\n");
        return 1;
      }
      else if(pid == 0) //filho
      {
        printf("sou o filho %d\n",getpid());

        for(int j = i*blocksize ; j < (i+1)*blocksize ; j++)
        {
          C[j] = A[j] + B[j];
        }

        exit(0);
      }
      else //pai
      {

      }
    } 
    while(wait(NULL) > 0);
    t2 = time(NULL);

    printf("time: %ld seconds\n",(t2-t1));

    //for(int i = 0 ; i < N ; i=i+50000)
    //  printf("C[%d] = %f\n",i,C[i]);

    shmdt(C);
    shmctl(shmid, IPC_RMID, NULL);

    free(A);
    free(B);

    return 0;
}
