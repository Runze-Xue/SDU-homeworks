#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include  <signal.h>
// #include <ncurses.h> // 使用 ncurses.h 头文件
#define MAX_LINE 100
/* 限定用户输入的命令行长度不超过 100 字符*/

char *MemoryOrders[100];
int memorystart = 0;
int memorycnt = 0;

void Decom(char *c, char *result[100],  char temp){//c -> result
    char cbreak[2]; cbreak[0] = temp; cbreak[1] = '\0';
    char* tok = strtok(c, cbreak);
    int cnt = 0;
    while(tok != NULL){
        int loc = 0;
        while(tok[loc] != '\0'){
            result[cnt][loc] = tok[loc];
            loc++;
        }
        if(tok[loc] =='\0')result[cnt][loc] = '\0';
        tok = strtok(NULL, cbreak);
        cnt++;
    }
    result[cnt] = NULL;
}

void Getmalloc(char* c[100]){
    for(int i = 0; i < 100; i++){
        c[i] = (char*)malloc(sizeof(char)*100);
    }
}
void Getfree(char *c[100]){
    for(int i = 0; i < 100; i++){
        free(c[i]);
    }
}
pid_t child_pid;
void sigint_handler(int sig_num) {
    /* 当接收到 SIGINT 信号时，终止子进程 */
    kill(child_pid, SIGINT);
    printf("\nChild process is terminated due to interrupt signal\n");
}
void sigquit_handler(int sig_num) {
    /* 当接收到 SIGQUIT 信号时，终止父进程 */
    printf("\nParent process is terminated due to quit signal\n");
    exit(0);
}
int main(void)
{   Getmalloc(MemoryOrders);
    int background; /* 如果命令行的最后字符是&，则 background=1 */
    int ordersize = 0;
    int ordersym = 0;
    while (1) {
    /* 父进程自定义信号 SIGINT 的处理程序，当按下 ctrl-c 后，
    * 不终止父进程（Shell），只终止其子进程。
    * 设置其它信号，当父进程（shell）收到该信号后，结束执行，退出
    * 
    // */  
    // printf("memorycnt%d\n",memorycnt);
        if(ordersize != 0)printf("Are you going to get memorys? \n");
        char c[100]; for(int i = 0; i < 100; i++)c[i] = '\0';
        memorycnt = memorystart;
        char ch;
        ordersym = 0;
        while(1){
            if(ordersize == 0) break;
             ch = getchar(); getchar();// /n
            if(ch == 'w'){//up botton
                // printf("in w %d\n", memorycnt);
                memorycnt--; if(memorycnt < 0) memorycnt = ordersize - 1;
                printf("%s\n",MemoryOrders[memorycnt]);
            }
            else if(ch == 's'){//down botton
                // printf("in s %d\n", memorycnt);
                memorycnt++; if(memorycnt >= ordersize) memorycnt = 0;
                printf("%s\n",MemoryOrders[memorycnt]);
            }
            else if(ch == 'y'){//enter botton
                // printf("in memoryorders\n");
                ordersym = 1;
                int cpycnt = 0;
                while(MemoryOrders[memorycnt][cpycnt] != '\0'){
                    c[cpycnt] = MemoryOrders[memorycnt][cpycnt];
                    // printf("ch == y%s\n",c);
                    cpycnt++;
                }
                c[cpycnt] = '\0';
                // memcpy(c, MemoryOrders[memorycnt], sizeof(c));
                break;
            }
            else if(ch == 'n'){
                ordersym = 0;
                break;
            }
            else continue;
        }
        // endwin();
        background=0;
        if(ordersym == 0){
            if(ordersize < 29)ordersize++;
            printf("COMMAND->"); /* 命令提示符*/
            char temp = getchar();
            int cnt = 0;
            while(temp!='\n'){
                c[cnt] = temp;
                temp = getchar(); cnt++;
            }
            if(strlen(c) > MAX_LINE){
                printf("Wrong Putin Length\n");
                continue;
            }
            // printf("c::: %s",c);
        }
        int copycnt = 0;
        memorycnt = memorystart;
        while(c[copycnt]!='\0'){
            MemoryOrders[memorycnt][copycnt] = c[copycnt];
            copycnt++;
        }
        MemoryOrders[memorycnt][copycnt] = '\0';
        // printf("Memoryorders2 :: %s\n",MemoryOrders[memorycnt]);
        memorystart++; memorystart %= 30;
        memorycnt = memorystart;
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, sigquit_handler);

        /* 读用户输入的命令行，并分析命令行 */
        if (c[strlen(c) - 1] =='&') /*命令行含有& 后台运行用户输入的命令*/
        { background =1; c[strlen(c) - 1] = '\0';}
        child_pid = fork();
        if (child_pid == 0) /* 处理执行命令行输入的命令 */
        {
        // signal(SIGINT,(__sighandler_t)sigcat);
        /* 设置信号 SIGINT 的处理方式为默认 SIG_DFL，
         signal(SIGINT, SIG_DFL);
        * 按下 ctrl-c 后，终止子进程 */
        /* 命令行中是否有 I/O 重定向，即双方有符号 >，< */
        char *command[100]; Getmalloc(command);
        char *commandleft[100];Getmalloc(commandleft);
        char *commandright[100];Getmalloc(commandright);
        // char *conmoncommand[100];Getmalloc(commandleft[100]);

        int newout = 0, newin = 0, crepipe = 0;
        for(int i = 0; i < strlen(c); i++){
            if(c[i] == '>'){newout = 1;break;}
            else if(c[i] =='<'){newin = 1;break;}
            else if(c[i] == '|'){crepipe = 1;break;}
        }
        char tempbreak = ' ';
        if(newout == 1) {tempbreak = '>';}
        else if(newin == 1) {tempbreak = '<'; }
        else if(crepipe == 1) {tempbreak = '|';}
        Decom(c, command,tempbreak);
        tempbreak = ' ';
        if(newout == 1){// command[1] is filename
            Decom(command[0], commandleft,tempbreak);
        }
        else if(newin == 1){// command[0] is filenam
            Decom(command[1], commandright, tempbreak);
        }
        else if(crepipe == 1){
            Decom(command[0], commandleft,tempbreak);
            Decom(command[1], commandright, tempbreak);
        }

        if (newout)/* if (输出重定向) */
        { /* 重定向标准输出到文件 outfile */
            int fd = creat(command[1],0666);
            close(1); /* close(stdout) */
            dup(fd);
            close(fd);
        } /* if (输出重定向) */
        else if (newin)/* if (输入重定向) */
        { /* 重定向文件 inputfile 到标准输入 */
            
            int fd = open(command[0],0666);
            close(0); /* close(stdin); */
            dup(fd);
            close(fd);
        } /* if (输入重定向) */
        /* 是否有形如 command1|command2 的管道 */
        else if (crepipe)/*需要建立管道“|”*/
        {   
            int fildes[2];
            pipe(fildes);
            if (fork()==0) /* 处理管道左部分 */
            {
            /* 标准输出到管道 */
                close(1);
                dup(fildes[1]);
                close(fildes[1]);
                close(fildes[0]);
                /* 执行管道左部分命令 */
                execvp(commandleft[0], commandleft);
            } /* 处理管道左部分 */
            /* 管道右部分 */
            /* 标准输入来自管道 */
            close(0);
            dup(fildes[0]);
            close(fildes[0]);
            close(fildes[1]);
        } /* if (需要建立管道“|”) */
        /* 如果输入的命令行有形如 command1|command2 的管道，
        * 这里就执行管道右部分命令。
        * 如果输入的命令行中没有管道符，
        * 则 command2 是用户在命令行中输入的命令
        */
        int exesym;
        if(newout == 1) {if(execvp(commandleft[0], commandleft) == -1) perror("execvp");}
        else if(newin == 1) {if(execvp(commandright[0], commandright) == -1) perror("execvp");}
        else if(crepipe == 1) {if(execvp(commandright[0], commandright) == -1)perror("execvp");} 
        else {if(execvp(command[0], command) == -1)perror("execvp") ;}
        Getfree(command);
        Getfree(commandleft);
        Getfree(commandright);

        } /* if (fork()= =0) 执行命令行输入的命令 */
        /* 父进程（即 shell）从此处执行 */
        /*
        * 如果不要求后台运行输入的命令，父进程等待子进程结束，
        * 否则，父进程不需要等待，继续执行，读入处理用户输入
        */
        int status;
        int retid;
        if (background == 0)
            retid=wait(&status);    
    } /* while (1) */
    free(MemoryOrders);
} /* main */