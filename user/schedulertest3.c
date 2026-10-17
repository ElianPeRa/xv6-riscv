#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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

  // high priority child: sleeps first, then wakes up
  if(fork() == 0){
    setpriority(30);
    printf("high: going to sleep for 10 ticks\n");
    pause(10);
    printf("high: woke up at tick %d\n", uptime());
    exit(0);
  }

  // low priority child: works the whole time
  if(fork() == 0){
    setpriority(10);
    printf("low: started at tick %d\n", uptime());
    work();
    printf("low: finished at tick %d\n", uptime());
    exit(0);
  }

  wait(0);
  wait(0);
  printf("test 3 done\n");
  exit(0);
}
