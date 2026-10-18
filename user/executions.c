#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  volatile int x = 0;
  int i;

  for(i = 0; i < 20000000; i++)
    x++;

  exit(0);
}
