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

    key_t key = 2002;
    int shm = shmget(key, 32, IPC_CREAT|0666);
    if(shm == -1){
        printf("shmget error \n");
        return 0;
    }

    int sm = shmget(key, 100, IPC_CREAT|0666);
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
        semop(shm, &close,1);
        time_t t = time(0);
        sprintf(p, "%s", ctime(&t));
        sleep(1);
        semop(shm, &open,1);
	}

	return 0;

}
