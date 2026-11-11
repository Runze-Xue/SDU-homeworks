#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void task1(int *); //线程 1 计算f(x)
void task2(int *); //线程 2 计算f(y)
void task3(); //线程 3 计算f(x+y)
int pipe1[2],pipe2[2], pipe3[2]; //存放3 无名管道标号
pthread_t thrd1,thrd2, thrd3; //存放3个线程标识
int main(int argc,char *arg[])
{
    int ret;
    int num1,num2;
    printf("please write x and y:\n");
    scanf("%d%d",&num1, &num2);
    //使用 pipe()系统调用建立3个无名管道。建立不成功程序退出，执行终止
    if(pipe(pipe1) < 0){
        perror("pipe1 not create");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe2) < 0)
    {
        perror("pipe2 not create");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe3) < 0)
    {
        perror("pipe3 not create");
        exit(EXIT_FAILURE);
    }
    //使用 pthread_create 系统调用建立3个线程。建立不成功程序退出，执行终止
    ret = pthread_create(&thrd1,NULL,(void *) task1,(void *) &num1);
    if(ret){
        perror("pthread_create: task1");
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&thrd2,NULL,(void *) task2,(void *) &num2);
    if(ret){
        perror("pthread_create: task2");
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&thrd3,NULL,(void *) task3,NULL);
    if(ret){
        perror("pthread_create: task3");
        exit(EXIT_FAILURE);
    }
    //挂起当前线程，等待线程 thrd1 结束，并回收其资源
    pthread_join(thrd1,NULL);
    //挂起当前线程，等待线程 thrd2 结束，并回收其资源
    pthread_join(thrd2,NULL);
    //挂起当前线程，等待线程 thrd2 结束，并回收其资源
    pthread_join(thrd3,NULL);

    exit(EXIT_SUCCESS);
    }
    // f(x)
void task1(int *num)
{
    int cnt = *num;
    int x = 1;
    int zero = 0;
    write(pipe1[1],&zero, sizeof(int));
    while(cnt){
        x *= cnt;
        cnt--;
        printf("now f(x) = %d\n", x);
    }
    printf("f(x) final answer is %d\n" ,x);
    write(pipe1[1],&x,sizeof(int));
    //读写完成后,关闭管道
    close(pipe1[1]);
}
//f(y)
void task2(int * num)
{   
    int zero = 0;
    write(pipe2[1],&zero, sizeof(int));
    if(*num == 1 ||*num == 2){
        int cnt = 1;
        write(pipe2[1],&cnt,sizeof(int));
        printf("f(y) final answer is 1\n");
    }   
    else{
        int cnt = 1; int last = 1;
        int count = 1;
        while(count <= *num - 2){
            int temp = cnt;
            cnt = last + cnt;
            last = temp;
            count++;
            printf("now f(y) = %d\n", cnt);
        }
        printf("f(y) final answer is%d\n",cnt);
        write(pipe2[1],&cnt,sizeof(int));
    }
        close(pipe2[1]);
    }

void task3()
{
    int x1, x2;
    read(pipe1[0],&x1,sizeof(int));
    read(pipe2[0],&x2,sizeof(int));
    while(1){
        read(pipe1[0],&x1,sizeof(int));
        read(pipe2[0],&x2,sizeof(int));
        printf("%d  %d\n",x1,x2);
        if(x1 > 0 && x2 > 0){
            printf("f(x+y) = %d\n",x1+x2);
            close(pipe1[0]);close(pipe2[0]);
            return;
        }
        else {
            printf("f(x+y) still waiting\n");
        }
    }
    close(pipe1[0]);
}