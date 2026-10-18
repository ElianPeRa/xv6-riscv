#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NJOBS 4

void
work(void)
{
  volatile int x = 0;
  int i;
  for(i = 0; i < 200000000; i++)
    x++;
}

int
main(int argc, char *argv[])
{
  int prios[NJOBS] = {0, 5, 10, 15};
  int startpipe[2], resultpipe[2];
  int result[2];
  int i, start, first, end;
  int totalresp = 0, totalturn = 0;

  setpriority(49);
  pipe(startpipe);
  pipe(resultpipe);

  // create the jobs; each one waits for the start signal
  for(i = 0; i < NJOBS; i++){
    if(fork() == 0){
      setpriority(prios[i]);
      read(startpipe[0], &start, sizeof(start));

      first = uptime();          // the job is running now
      work();
      end = uptime();

      result[0] = first - start; // response time
      result[1] = end - start;   // turnaround time
      printf("job %d (priority %d): response %d, turnaround %d\n",
             i, prios[i], result[0], result[1]);
      write(resultpipe[1], result, sizeof(result));
      exit(0);
    }
  }

  pause(5);   // let every job set its priority and start waiting

  // start all jobs at the same moment
  start = uptime();
  for(i = 0; i < NJOBS; i++)
    write(startpipe[1], &start, sizeof(start));

  for(i = 0; i < NJOBS; i++)
    wait(0);

  // collect the results and compute the averages
  for(i = 0; i < NJOBS; i++){
    read(resultpipe[0], result, sizeof(result));
    totalresp += result[0];
    totalturn += result[1];
  }

  printf("average response time:   %d.%d ticks\n",
         totalresp / NJOBS, (totalresp * 10 / NJOBS) % 10);
  printf("average turnaround time: %d.%d ticks\n",
         totalturn / NJOBS, (totalturn * 10 / NJOBS) % 10);
  exit(0);
}
