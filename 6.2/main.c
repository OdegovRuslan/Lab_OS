#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

pthread_rwlock_t cl;

void* f(){
    key_t key = ftok("sm", 30);
    int shm = shmget(key, 100, 0666);
    if (shm == -1){
        printf("shmget error \n");
        return 0;
    }

    char* p = shmat(shm, NULL, 0);
    if(p == (char*)-1){
		printf ("shmat error\n");
		return 0;
	}

    time_t t;
    while(1){
		t = time(0);
        pthread_rwlock_wrlock(&cl);
        sprintf(p, "%s", ctime(&t));
        pthread_rwlock_unlock(&cl);
		sleep (1);
	}

}

int main(){

    pthread_t pth;
    pthread_rwlock_init (&cl, NULL);

    if(pthread_create(&pth, NULL, f, NULL) != 0){
        printf ("pthread_create error\n");
		return 0;
    }

    key_t key = ftok("sm", 30);
    int shm = shmget(key, 100, IPC_CREAT|0666);
    if (shm == -1){
        printf("shmget error \n");
        return 0;
    }

    char* p = shmat(shm, NULL, 0);
    if(p == (char*)-1){
		printf ("shmat error\n");
		return 0;
	}

    while(1){
        pthread_rwlock_rdlock (&cl);
        printf ("%s\n", p);
        pthread_rwlock_unlock (&cl);
        sleep(1);
	}

    pthread_rwlock_destroy(&cl);
    return 0;
}
