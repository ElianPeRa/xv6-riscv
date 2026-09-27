#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
int startTicks;
int endTicks;
int elapsedTicks;
int pid;
//checkin if the user entered a commmand
if(argc<2){
fprintf(2,"usage: time1 command [args], no command was entered\n");
exit(1);
}
startTicks=uptime();
pid=fork();

if(pid<0){
fprintf(2,"time1: fork has failed :(\n");
exit(1);
}
if(pid==0){
//child process is replaced with the new command, argv[1]
//is the command name, &argv passes the command with arguments
exec(argv[1],&argv[1]);

fprintf(2,"time1: exec() has failed for %s\n",argv[1]);
exit(1);
}
wait(0);
endTicks=uptime();
elapsedTicks=endTicks-startTicks;
printf("elapsed time: %d ticks\n",elapsedTicks);
exit(0);
}

