#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(){
    struct sembuf close = {0,-1,0};
    struct sembuf open = {0,1,0};

    key_t key = ftok("/tmp", 'a');
    int shm = shmget(key, NULL, IPC_CREAT|0666);
    if (shm == -1){
        printf("shmget error \n");
        return 0;
    }

    int sm = shmget(key, NULL, IPC_CREAT|0666);
    if (sm == -1){
        printf("shmget error \n");
        return 0;
    }

    char* p = shmat(shm, NULL, 0);
    if(p == (char*)-1){
		printf ("shmat error\n");
		return 0;
	}

	while (1){
        semop(sm, &close, 1);
        printf("%s", p);
        sleep (1);

        semop(sm, &open, 1);
	}

	return 0;
}
