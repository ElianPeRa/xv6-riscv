#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// waste CPU time
void
work(void)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < 300000000; i++)
    x++;
}

int
main(int argc, char *argv[])
{
  setpriority(49);

  // first child: low priority
  if(fork() == 0){
    setpriority(10);
    work();
    printf("low priority (10) child finished\n");
    exit(0);
  }

  // second child: high priority
  if(fork() == 0){
    setpriority(30);
    work();
    printf("high priority (30) child finished\n");
    exit(0);
  }

  wait(0);
  wait(0);
  printf("test 1 done\n");
  exit(0);
}
