#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{
	DIR *d;
	int n = 0;
	struct dirent *dir = (struct dirent*)calloc(sizeof (struct dirent)+1, 1);
	struct dirent **list;
	struct stat *buf;
	time_t t;

	buf = (struct stat*)calloc(sizeof(struct stat), 1);
	n = scandir (".", &list, NULL, alphasort);
	int i=0;

	while(1){
		dir = list[i];
		if(!dir) break;

		stat(dir->d_name, buf);

		printf((buf->st_mode & S_IFDIR)?"d":"-");
		printf((buf->st_mode & S_IRUSR)?"r":"-");
		printf((buf->st_mode & S_IWUSR)?"w":"-");
		printf((buf->st_mode & S_IXUSR)?"x":"-");
		printf((buf->st_mode & S_IRGRP)?"r":"-");
		printf((buf->st_mode & S_IWGRP)?"w":"-");
		printf((buf->st_mode & S_IXGRP)?"x":"-");
		printf((buf->st_mode & S_IROTH)?"r":"-");
		printf((buf->st_mode & S_IWOTH)?"w":"-");
		printf((buf->st_mode & S_IXOTH)?"x":"-");

		char mtime[80];
		t = buf->st_mtime;
		struct tm lt;
		localtime_r(&t, &lt);
		strftime(mtime, sizeof mtime, "%d %b %Y %T", &lt);
		printf("\t%s", mtime);

		printf(" %s\n", dir->d_name);
		free(list[n]);
		i++;
	}
	return(0);
}
