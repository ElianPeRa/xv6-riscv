#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;

  printf("priority at start: %d\n", getpriority());

  // try a bad value, it should fail
  if(setpriority(60) < 0)
    printf("setpriority(60) failed\n");

  // set a good value
  setpriority(20);
  printf("priority now: %d\n", getpriority());

  pid = fork();
  if(pid == 0){
    // child
    printf("child priority 20: %d\n", getpriority());
    setpriority(30);
    printf("child priority after change: %d\n", getpriority());

    // turn this child into ps
    char *args[] = {"ps", 0};
    exec("ps", args);
    printf("failed exec\n");
    exit(1);
  }

  // parent waits for the child (ps) to finish
  wait(0);
  printf("parent priority is still: %d\n", getpriority());
  exit(0);
}
