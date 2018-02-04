/* process_ring.h */
#include <xinu.h>


process process_ring(int32, volatile int32*, volatile int32[]);
extern  int32 r;
extern  int32 p;
extern volatile int32 roundNo;
process process_ring_msg(int32, volatile pid32[], pid32);
process process_hello();
