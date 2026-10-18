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
  int start;

  setpriority(49);
  start = uptime();

  // high priority child: works the whole time
  if(fork() == 0){
    setpriority(10);
    work();
    printf("high (10): finished at tick +%d\n", uptime() - start);
    exit(0);
  }

  // low priority child: when does it first get the CPU?
  if(fork() == 0){
    setpriority(0);
    pause(2);
    printf("low (0): got the CPU at tick +%d\n", uptime() - start);
    work();
    printf("low (0): finished at tick +%d\n", uptime() - start);
    exit(0);
  }

  wait(0);
  wait(0);
  exit(0);
}
