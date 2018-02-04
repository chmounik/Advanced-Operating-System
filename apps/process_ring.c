#include <process_ring.h>
#include <xinu.h>

process process_ring(int32 i,volatile int32 *countdown,volatile int32 mailbox[])
{
	int32 last = 0;
	while (last < r) {
	if(mailbox[i] != -1)
	{
		printf("Ring Element %d : Round %d : Value : %d\n",i, roundNo, mailbox[i]);
		mailbox[(i+1)%p] = mailbox[i] - 1;
        if(p>1)
		mailbox[i] = -1;
		*countdown -= 1;
		last += 1;
		if(p == i+1)
		{
			roundNo += 1;
		}
	}
	}
	return OK;
}

process process_ring_msg(int32 i,volatile pid32 msgpass[],pid32 parent_pid)
{
	int32 last = 0,count = 9999;
    while (last < r) {
        count = receive();
        printf("Ring Element %d : Round %d : Value : %d\n",i, roundNo, count);
	if(p == i+1)
        {
            roundNo += 1;
        }
        if(count != 0)
	send(msgpass[(i+1)%p],count - 1);
        last += 1;
    }
    	return OK;
}


