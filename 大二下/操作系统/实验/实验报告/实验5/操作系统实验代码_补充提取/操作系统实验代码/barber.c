#include "ipc.h"

int main(){
    Msg_buf msg_arg;
    quest_key = 201;
    quest_flg = IPC_CREAT | 0644;
    quest_id = set_msq(quest_key, quest_flg);
    semsofakey = 401;
    semsofaflg = IPC_CREAT | 0644; 
    sofasem = set_sem(semsofakey, 1, semsofaflg);
    down(sofasem);
    semmoneykey = 501;
    semmoneyflg = IPC_CREAT | 0644;
    moneysem = set_sem(semmoneykey, 1, semmoneyflg);
    lockkey = 601;
    semlockflg = IPC_CREAT | 0644;
    locksem = set_sem(lockkey, 1, semlockflg);
    buff_key = 101;
    buff_num = 1;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    sofa_numkey = 102;
    sofa_num = 1;
    shm_flg = IPC_CREAT | 0644;
    sofa_numptr = (char *)set_shm(sofa_numkey,sofa_num,shm_flg);
    int pid1 = fork();
    if(pid1 == 0){
        int pid2 = fork();
        if(pid2 == 0){
            while(1){
                int my_pid = getpid();
                printf("barber %d go to sofa and sleep \n", my_pid);
               // printf("semctl %d\n", semctl(locksem));
                down(sofasem);
                down(locksem);
                sofa_numptr[0]--;
                // printf("%d\n" ,sofa_numptr[0]);
                up(locksem);
                msgrcv(quest_id, &msg_arg, sizeof(msg_arg), BARBERQUEST, 0);
                printf("barber %d take guest %d to cut hair\n", my_pid, msg_arg.mid);
                sleep(7);
                down(moneysem);
                buff_ptr[0]++;
                printf("barber %d get 1 money from %d and we get %d in total \n", my_pid, msg_arg.mid, buff_ptr[0]);
                up(moneysem);
            }
        }
        else{    
            while(1){
                int my_pid = getpid();
                printf("barber %d go to sofa and sleep \n", my_pid);
                down(sofasem);
                down(locksem);
                sofa_numptr[0]--;
                // printf("%d\n" ,sofa_numptr[0]);
                up(locksem);
                msgrcv(quest_id, &msg_arg, sizeof(msg_arg), BARBERQUEST, 0);
                printf("barber %d take guest %d to cut hair\n", my_pid, msg_arg.mid);
                sleep(7);
                down(moneysem);
                buff_ptr[0]++;
                printf("barber %d get 1 money from %d and we get %d in total \n", my_pid, msg_arg.mid, buff_ptr[0]);
                up(moneysem);
            }
        }
        // waitpid();
    }
    else {  
            while(1){
                int my_pid = getpid();
                printf("barber %d go to sofa and sleep \n", my_pid);
                down(sofasem);
                down(locksem);
                sofa_numptr[0]--;
                // printf("%d\n" ,sofa_numptr[0]);
                up(locksem);
                msgrcv(quest_id, &msg_arg, sizeof(msg_arg), BARBERQUEST, 0);
                printf("barber %d take guest %d to cut hair\n", my_pid, msg_arg.mid);
                sleep(7);
                down(moneysem);
                buff_ptr[0]++;
                printf("barber %d get 1 money from %d and we get %d in total \n", my_pid, msg_arg.mid, buff_ptr[0]);
                up(moneysem);
            }    }
    // waitpid();
}
