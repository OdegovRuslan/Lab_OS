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


pthread_mutex_t mutex;

void* f(){
    key_t key = ftok("/tmp", 'a');
    int shm = shmget(key, NULL, IPC_CREAT|0666);
    if (shm == -1){
        printf("shmget error \n");
        return 0;
    }

    char* p = shmat(shm, NULL, 0);
    if(p == (char*)-1){
		printf ("shmat error\n");
		return 0;
	}

    time_t t = time(0);
    while(1){
		t = time(0);
        pthread_mutex_lock(&mutex);
        sprintf(p, "%s", ctime(&t));
        pthread_mutex_unlock(&mutex);
		sleep (1);
	}

}

int main(){

    pthread_t pth;
    pthread_mutex_init (&mutex, NULL);

    if(pthread_create(&pth, NULL, f, NULL) != 0){
        printf ("pthread_create error\n");
		return 0;
    }

    key_t key = ftok("/tmp", 'a');
    int shm = shmget(key, NULL, IPC_CREAT|0666);
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
        pthread_mutex_lock (&mutex);
        printf("%s", p);
        pthread_mutex_unlock(&mutex);
        sleep(1);
	}

    pthread_mutex_destroy(&mutex);
    return 0;
}
