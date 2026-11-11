/*
Filename
copyright
Function
: producer.c
: (C) 2006 by zhonghonglie
: 建立并模拟生产者进程
*/
#ifndef _IPC_H
#define _IPC_H
#include "ipc.h"
#endif

int main(int argc,char *argv[])
    {
    printf("in");
    int rate;
    //可在在命令行第一参数指定一个进程睡眠秒数，以调解进程执行速度
    if(argv[1] != NULL) rate = atoi(argv[1]);
    else rate = 3;

    //不指定为 3 秒
    //共享内存使用的变量
    buff_key = 101;//缓冲区任给的键值
    buff_num = 8;//缓冲区任给的长度
    pput_key = 102;//生产者放产品指针的键值
    pput_num = 1; //指针数
    shm_flg = IPC_CREAT | 0644;//共享内存读写权限
    //获取缓冲区使用的共享内存，buff_ptr 指向缓冲区首地址

    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    //获取生产者放产品位置指针 pput_ptr

    pput_ptr = (int *)set_shm(pput_key,pput_num,shm_flg);
    //信号量使用的变量

    prod_key = 201;//生产者同步信号量键值
    pmtx_key = 202;//生产者互斥信号量键值
    cons_key = 301;//消费者同步信号量键值
    cmtx_key = 302;//消费者互斥信号量键值
    prok[0] = 501; 
    prok[1] = 502;
    prok[2] = 503;

    // printf("2in2in2in");
    sem_flg = IPC_CREAT | 0644;
    //生产者同步信号量初值设为缓冲区最大可用量 sem_val = buff_num;
    //获取生产者同步信号量，引用标识存 prod_sem

    sem_val = buff_num;

    prod_sem = set_sem(prod_key,sem_val,sem_flg);
    sem_val = 0;
    pro[0] = set_sem(prok[0], sem_val, sem_flg);
    sem_val = 0;
    pro[1] = set_sem(prok[1], sem_val, sem_flg);
    sem_val = 0;
    pro[2] = set_sem(prok[2], sem_val, sem_flg);
    //消费者初始无产品可取，同步信号量初值设为 0

    sem_val = 0;
    //获取消费者同步信号量，引用标识存 cons_sem
    cons_sem = set_sem(cons_key,sem_val,sem_flg);
    //生产者互斥信号量初值为 1
    sem_val = 1;
    //获取生产者互斥信号量，引用标识存 pmtx_sem
    pmtx_sem = set_sem(pmtx_key,sem_val,sem_flg);
    
    // printf("asdf");
    //循环执行模拟生产者不断放产品
        while(1){
        //如果缓冲区满则生产者阻塞
        down(prod_sem);
        //如果另一生产者正在放产品，本生产者阻塞
        down(pmtx_sem);
        //用写一字符的形式模拟生产者放产品，报告本进程号和放入的字符及存放的位置
        int checkpacage = 0;
        buff_ptr[*pput_ptr] = 'A'+ (*pput_ptr)%3;
        if('A'+ (*pput_ptr)%3 -'A' == 0) checkpacage += 1;
        else if('A'+ (*pput_ptr)%3 -'A' == 1) checkpacage += 2;
        else if('A'+ (*pput_ptr)%3 -'A' == 2) checkpacage += 4;
        sleep(rate);
        printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);
        up(pro['A'+ (*pput_ptr)%3 -'A']);
        *pput_ptr = (*pput_ptr+1) % buff_num;
        down(prod_sem);
        //存放位置循环下移
        buff_ptr[*pput_ptr] = 'A'+ (*pput_ptr)%3;
        if('A'+ (*pput_ptr)%3 -'A' == 0) checkpacage += 1;
        else if('A'+ (*pput_ptr)%3 -'A' == 1) checkpacage += 2;
        else if('A'+ (*pput_ptr)%3 -'A' == 2) checkpacage += 4;    
        sleep(rate);
        printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);
        up(pro['A'+ (*pput_ptr)%3 -'A']);
        *pput_ptr = (*pput_ptr+1) % buff_num;
        if(checkpacage == 3) up(pro[2]);
        else if(checkpacage == 5)up(pro[1]);
        else if(checkpacage == 6)up(pro[0]);
        //唤醒阻塞的生产者
        up(pmtx_sem);
        //唤醒阻塞的消费者
        up(cons_sem);
    }
    return EXIT_SUCCESS;
}