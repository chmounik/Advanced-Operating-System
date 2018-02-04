#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int nargs, char *args[]) {
	int pd[2];
	pid_t pid;
	//check that no more than 2 arguments ae given
	if(!(nargs == 2))
	{
		perror("Incorrect number of arguments\n");
		printf("Usage: sys_info CMD\n");
		return (-1);
	}
	else{
		char *cmd = args[1];
		//pipe descriptors for synchronization
		pipe(pd);
		//create a child process
		pid = fork();
		// pid returns -1 if fork did not happen
		if(pid == -1)
		{
			perror("fork");
        		_exit(1);
		}
		//child process if pid = 0
		else if (pid == 0) {
			printf("Child PID = %d\n",getpid());
			//close the parent pipe descriptor
			close(pd[1]);
			// read the argument from parent process
			read(pd[0], &cmd, (strlen(cmd)+1));
			//close the child pipe descriptor
			close(pd[0]);
			// compare the read string with echo and if its equal echo Hello World
			if(strncmp(cmd,"echo",4) == 0)
				execl("/bin/echo","echo","Hello World!!",NULL);
			else
				execlp(cmd,cmd,NULL);
			_exit(0);
		} 
		else {
			printf("Parent PID = %d\n",getpid());
			close(pd[0]);
			write(pd[1], &cmd, (strlen(cmd)+1));
			wait(NULL);
			close(pd[1]);
			_exit(0);	
		}
	return 0;
	}
}
