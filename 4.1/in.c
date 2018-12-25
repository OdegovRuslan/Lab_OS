#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(){

    key_t key = ftok("/tmp", 'a');
    int shm = shmget(key, 2*sizeof(int), IPC_CREAT|0666);
    if (shm == -1){
        printf("Error \n");
        return 0;
    }

    char* p = shmat(shm, NULL, 0);
    if(p == (char*)-1){
		printf ("Shmat error\n");
		return 0;
	}

    time_t t = time(0);
    sprintf(p, "%s", ctime(&t));

    return 0;
}
