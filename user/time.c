#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"


int main(int argc, char *argv[]){
int startTicks;
int endTicks;
int elapsedTicks;
int pid;
int isOut;
int cpuPercent;
struct rusage usage;

//checkin if the user entered a commmand
if(argc<2){
fprintf(2,"usage: time command [args..]\n");
exit(1);
}
startTicks=uptime();
pid=fork();

if(pid<0){
fprintf(2,"time: fork has failed :(\n");
exit(1);
}
if(pid==0){
//child process is replaced with the new command, argv[1]
//is the command name, &argv passes the command with arguments
exec(argv[1],&argv[1]);

fprintf(2,"time: exec has failed for %s\n",argv[1]);
exit(1);
}
//parent is supposed to wait for the child and gets its cpu time
wait2(&isOut,&usage);
endTicks=uptime();
elapsedTicks=endTicks-startTicks;

//% of the child time elapsed on the cpu
if(elapsedTicks>0){
cpuPercent=(usage.cputime*100)/elapsedTicks;
}else{
cpuPercent=0;
}

printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",elapsedTicks,usage.cputime,cpuPercent);
exit(0);
}

