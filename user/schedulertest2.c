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

  // low priority child: just works
  if(fork() == 0){
    setpriority(10);
    work();
    exit(0);
  }

  // high priority child: works, then runs ps
  if(fork() == 0){
    setpriority(30);
    work();
    printf("high child finished its work, running ps:\n");
    char *args[] = {"ps", 0};
    exec("ps", args);
    printf("exec failed\n");
    exit(1);
  }

  wait(0);
  wait(0);
  printf("test 2 done\n");
  exit(0);
}
