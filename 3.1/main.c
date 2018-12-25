#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main (){

    int fd[2];

    if(pipe(fd) < 0){
        printf ("Error create pipe\n");
        return 0;
    }

    pid_t f;
    switch(f = fork()){
        case -1:
            printf ("Error fork\n");
            break;
        case 0:
        {
            time_t tp;
            close(fd[1]);
            sleep(1);
            read(fd[0], &tp, sizeof(time_t));
            printf("PARENT: Received data from pipe\n");
            close(fd[0]);

            printf ("%s\n", ctime(&tp));
            break;
        }
        default:
        {
            close(fd[0]);

            time_t tc = time (0);
            write(fd[1], &tc, sizeof(time_t));
            sleep(1);
            printf("CHILD: Put data in pipe\n");
            close(fd[1]);
            break;
        }
    }
    return 0;
}
