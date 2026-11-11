#include "ipc.h"

int main(){
    Msg_buf msg_arg;
    quest_key = 201;
    quest_flg = IPC_CREAT | 0644;
    quest_id = set_msq(quest_key, quest_flg);
    semsofakey = 401;
    semsofaflg = IPC_CREAT | 0644; 
    sofasem = set_sem(semsofakey,0, semsofaflg);

    long long guestnumber = 0;
    sofa_numkey = 102;
    sofa_num = 1;
    shm_flg = IPC_CREAT | 0644;
    sofa_numptr = (char *)set_shm(sofa_numkey,sofa_num,shm_flg);
    sofa_numptr[0] = 0;
    buff_key = 101;
    buff_num = 1;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    buff_ptr[0] = 0;
    while( msgrcv(quest_id, &msg_arg, sizeof(msg_arg), BARBERQUEST, IPC_NOWAIT) >= 0);
    lockkey = 601;
    semlockflg = IPC_CREAT | 0644;
    locksem = set_sem(lockkey, 1, semlockflg);
    while(1){
        // printf("sofa_numptr[0]  %d\n",sofa_numptr[0]);
        sleep(1);
        if(waitroomnum + sofa_numptr[0] > 20) continue;
        else if(sofa_numptr[0] < 4){
            guestnumber++;
            printf("guest %lld come to sofa \n", guestnumber);
            msg_arg.mid = guestnumber;
            msg_arg.mtype = BARBERQUEST;
            msgsnd(quest_id, &msg_arg, sizeof(msg_arg), IPC_NOWAIT);
            if(waitroomnum > 0){
                waitroomnum--;
                up(sofasem);
                // printf("ww1\n");
                down(locksem);
                // printf("ww2\n");
                sofa_numptr[0]++;
                up(locksem);
            }
            else {
                up(sofasem);
                // printf("ww1\n");
                down(locksem);
                // printf("ww2\n");
                sofa_numptr[0]++;
                up(locksem);
            }
        }
        else if(waitroomnum < 17) {
            waitroomnum++;
            printf("a new guest come\n");
        }
        else printf("lose a guest\n");
    }
    return 0;
}