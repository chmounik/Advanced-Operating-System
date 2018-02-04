/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_hello - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[])
{
	// to check for correct number of arguments
	if(nargs == 2){
		printf("Hello %s, Wecome to the world of Xinu!!\n", args[1]);
		kill(currpid);
		return 0;
		}
	//when more than 2 arguments are given
	else if(nargs > 2)
		{
		printf("Too many Arguments\n");
		return 1;
		}
	//less than 2 arguments given
	else{
		printf("Too few Arguments\n");
		return 1;
		}
}
