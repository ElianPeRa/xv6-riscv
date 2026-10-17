#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  char *state;
  int current=uptime();
  static char *states[] = {
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running ",
    [ZOMBIE]    "zombie  "
  };

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  printf("pid\tstate\tprio\tage\tsize\tppid\tname\n");
  for (i=0; i<nprocs; i++) {
    state = states[uproc[i].state];
//changes in print: %d\t was added to print unproc[i].priority, also
    if(uproc[i].state == RUNNABLE){
      int age = current - uproc[i].readytime;
      printf("%d\t%s\t%d\t%d\t%ld\t%d\t%s\n", uproc[i].pid, state, uproc[i].priority,
             age, uproc[i].size, uproc[i].ppid, uproc[i].name);
    } else {
      printf("%d\t%s\t%d\tno aging\t%ld\t%d\t%s\n", uproc[i].pid, state, uproc[i].priority,
             uproc[i].size, uproc[i].ppid, uproc[i].name);
    }

//printf("%d\t%s\t%d\t%ld\t%d\t%s\n", uproc[i].pid, state,
      //             uproc[i].priority,uproc[i].size, uproc[i].ppid, uproc[i].name);
  }

  exit(0);
}
