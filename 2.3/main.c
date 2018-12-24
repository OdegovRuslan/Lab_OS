#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#define FILE_SIZE 100000000
#define BUFFER_SIZE 100000

int generate_source_file()
{
	int f = open("read.txt", O_RDWR | O_TRUNC | O_CREAT | O_NONBLOCK, S_IWRITE|S_IREAD);
	char* buf = (char*)calloc(BUFFER_SIZE, sizeof(char));

	for(int i = 0; i < BUFFER_SIZE; i++)
		buf[i] = '0' + i % 10;

	if(f < 0){
		printf("error open file 1\n");
		return -1;
	}
	else{
		for(int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++)
			write(f, buf, BUFFER_SIZE);
	}

	close(f);
	return 0;
}


int source_to_target_copy()
{
    int fread = open("read.txt", O_RDONLY | O_CREAT);
	int fwrite = open("write.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE|S_IREAD);

	if(fwrite < 0){
		printf("error open file 2\n");
		return -1;
	}
	if(fread < 0){
        printf("error open file 3\n");
        return -1;
    }

	fd_set fdread;
	fd_set fdwrite;

	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);

    FD_SET(fread, &fdread);
	FD_SET(fwrite, &fdwrite);

    struct timeval t;
    t.tv_sec = 3;
	t.tv_usec = 0;

	int max = (fread > fwrite)? fread : fwrite;
	char* buf_1 = (char*)calloc(BUFFER_SIZE, sizeof(char));
    int ret = select(max + 1, &fdwrite, &fdread, NULL, &t);
	if(ret == -1 ){
        printf("error select\n");
		return -1;
	}

	ssize_t buf_2;

    if(FD_ISSET(fread, &fdread)){
        for(int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++){
             buf_2 = read(fread, buf_1, BUFFER_SIZE);
             write(fwrite, buf_1, buf_2);
        }
	}

	close(fread);
	close(fwrite);
	return(0);
}

int main(void)
{
	pid_t pid = fork();
	switch(pid){
	    case -1:{
            printf("error fork\n");
            return 0;
	    }
		case 0:{
            sleep(1);
            if(source_to_target_copy() == -1){
                return 0;
            }
        }
        default:{
            if(generate_source_file() == -1){
                return 0;
            }
        }
	}
	return 0;
}
