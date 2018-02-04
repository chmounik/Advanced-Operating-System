#include <xinu.h>
#include <stdlib.h> /* strcmp, strtol */
#include <stdio.h>
#include <process_ring.h>

void print_usage(void){
    printf("Usage: ...\n");
}
int32 p,currprio;
int32 r;
volatile int32 roundNo;
struct procent *currptr;
shellcmd xsh_process_ring(int argc, char *args[])
{
	if (argc == 2 && (0 == strncmp("--help", args[1], 7))) {
   		printf("Usage-- %s\n", args[0]);
    		printf("\t-p\tnumber of processes\n");
    		printf("\t-r\tnumber of rounds\n");
    		printf("\t-i\t sync or poll\n");
	    	printf("\t--help\thelp\n");
    		return SHELL_OK;
 		}
    p = 2; /* specified default */
    r = 3;
    roundNo = 0;
    int32 i,j,k;
    char sync = 'p';
    uint32 before;
    uint32 after;
    currptr =&proctab[currpid];
    for(i = 1; i < argc; i++){
        if (0 == strncmp("-p", args[i], 3)){
            /* parse numeric argument to -p */
            if (! (i + 1 < argc)) {
                /* No argument after flag */
                print_usage();
                printf("-p flag expected an argument\n");
                return SHELL_ERROR;
            }
            p = atoi(args[i + 1]);
            if (!(0 <= p && p <= 64)) {
                /* The number is out of range */
                print_usage();
                printf("-p flag expected a number between 0 - 64\n");
                return SHELL_ERROR;
            }
            /* Skip pass the numeric argument,
             *          that was successfully parsed */
            i += 1;
        }
        else if (0 == strncmp("-r", args[i], 3)) {
            if (! (i + 1 < argc)) {
                /* No argument after flag */
                print_usage();
                printf("-r flag expected an argument\n");
                return SHELL_ERROR;
            }
            r = atoi(args[i + 1]);
            if (!(r > 0)) {
                print_usage();
                printf("-r flag expected a number greater than 0\n");
                return SHELL_ERROR;
            }
            i += 1;
        }
        else if(0 == strncmp("-i", args[i], 3)) {
            if(0 == strncmp("sync", args[i+1], 4))
            {
                sync = 's';
            }
            else if(!(0 == strncmp("poll", args[i+1], 4)))
            {
                print_usage();
                printf("-i flag expected either POLL or SYNC\n");
                return SHELL_ERROR;
            }
            i += 1;
        }
        else{
            print_usage();
            printf("invalid flag");
        }
    }
    printf("Number of Processes: %d\n",p);
    printf("Number of Rounds: %d\n",r);
    volatile int32 countdown = (p * r) - 1;
    /* Polling method */
	if(sync == 'p')
    	{
        volatile int32 mailbox[p];
        mailbox[0] = countdown;
        for(k=1;k<p;k++)
            mailbox[k] = -1;
	gettime(&before);
	currprio = currptr->prprio;
        for(j=0;j<p;j++)
        {
		kprintf("\n%d\n",currprio);
		resume(create(process_hello,128,currprio+5,"hello",0));
            resume(create(process_ring,1024,20,"polling",3,j,&countdown,mailbox));
        }
    while(countdown >= 0);
	gettime(&after);
	printf("Elapsed Seconds: %d\n",(after - before));
    }
	/* Message Passing */
    else if(sync == 's')
    {
	pid32 parent_pid = getpid();
        volatile pid32 msgpass[p];
	gettime(&before);
        for(j=0;j<p;j++)
        {
            msgpass[j] = create(process_ring_msg,1024,20,"messagepassing",3,j,msgpass,parent_pid);
            resume(msgpass[j]);
        }
        send(msgpass[0],countdown);
	while(roundNo < r);
	gettime(&after);
	printf("Elapsed Seconds: %d\n",(after - before));
    }
    return SHELL_OK;
}
