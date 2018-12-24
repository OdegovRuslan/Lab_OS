#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SIZE_FILE 104858000

int main(){

    pid_t pid = fork();
    switch (pid){
        case -1:
        {
            printf("error fork\n");
            return 0;
            break;
        }
        case 0:
        {
            sleep (2);

            int f = open("Fout", O_RDONLY);
            if(f < 0){
                printf("error open\n");
                return 0;
            }

            int fc = open("Fin", O_CREAT|O_TRUNC|O_WRONLY);
            if(f < 0){
                printf("error open\n");
                return 0;
            }

            char * buf = (char*)calloc(1000, sizeof(char));

            while(read(f, buf, 1000))
                write (fc, buf, 1000);

            printf("file copy");
            break;
        }
        default:
        {
            char * buf = (char*)calloc(1000, sizeof(char));
            for (int i=0; i < 1000; i++)
            buf[i] = '0' + i % 10;

            int f = open("Fout", O_RDWR|O_TRUNC|O_CREAT);
            if(f < 0){
                printf("error open\n");
                return 0;
            }

            for(int i = 0; i < SIZE_FILE / 1000; i++)
                write(f, buf, 1000);

            printf("file is ready");
            break;
        }
    }

    return 0;
}
