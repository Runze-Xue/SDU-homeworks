#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> //进程自定义的键盘中断信号处理函数 typedef void (*sighandler_t) (int);

void sigcat(){
printf("%d Process continue\n",getpid());
}

int main(){

    signal(SIGINT,(__sighandler_t)sigcat);

    char *argv[] ={NULL};
    while(1){
    int pid1 = fork();
    if(pid1 == 0){
        pause();
        execvp("ls",argv);
    }
    else{
        int pid2 = fork();
        if(pid2 == 0){
         execvp("ps",argv);
        }
        int status2, status1;
        waitpid(pid2,&status2, 0);
        kill(pid1, SIGINT);
        waitpid(pid1,&status1, 0);
    }
    sleep(3);
    }
}