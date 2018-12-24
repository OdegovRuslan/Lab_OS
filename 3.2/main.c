#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(){

    unlink("fifo0001");
    if((mkfifo("fifo0001", 0777)) == -1){
        printf("FIFO Create error\n");
        return 0;
    }

    pid_t f;
    switch(f = fork()){
        case -1:
            printf("Error fork\n");
            break;
        case 0:
        {
            int fp = open("fifo0001", O_RDONLY);
            if(fp == -1){
                printf("Error open fifo\n");
                return 0;
            }

            char *tp = (char*)calloc(80, sizeof (char));
            read(fp, tp, 80);
            close(fp);
            printf("%s\n", tp);
            break;
        }
        default:
        {
            int fc = open("fifo0001", O_WRONLY);
            if(fc == -1){
                printf("Error open fifo\n");
                return 0;
            }

            time_t t = time (0);
            write(fc, ctime(&t), 80);
            close(fc);
            break;
        }
    }
    return 0;
}
