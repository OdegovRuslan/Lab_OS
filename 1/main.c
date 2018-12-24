#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t f;
	int err=0;

	for (int i = 0; i < 3; i++)
    {
        switch (f = fork())
        {
			case -1:
				printf ("Error\n");
				break;
			case 0:
				printf ("C My pid - %x, my parent - %x, lvl: %d\n",getpid(), getppid(), i);
				break;
			default:
				printf ("P My pid - %x, my parent - %x, lvl: %d\n", getpid(), getppid(), i);
				waitpid (f, &err, WUNTRACED);
			break;
        }
    }

	return 0;
}
