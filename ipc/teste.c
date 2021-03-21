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

	int *X;

	k = ftok("/mnt/disco1b/unb/sor/ipc/soma_ipc.c",'R');
	shmid = shmget(k, 1*sizeof(int), 0644|IPC_CREAT);

	X = shmat(shmid, (void*)0, 0); //pai associa-se a regiao compartilhada
	if(X == (int *)(-1))
		fprintf(stderr,"erro, nao foi possivel associar (shmat)\n");

	*X = 0;
	pid = fork();
	if(pid < 0)
	{
		fprintf(stderr,"erro, nao eh possivel criar processo filho\n");
		return 1;
	}
	else if(pid == 0) //filho
	{
		*X = 1;
		printf("sou o filho %d\n",getpid());
		printf("X no filho = %d\n",*X);
		exit(0);
	}
	else //pai
	{
		*X = 2;
		printf("sou o pai %d\n",getpid());
		printf("X no pai= %d\n",*X);
	}

	while(wait(NULL) > 0);

	shmdt(X);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
