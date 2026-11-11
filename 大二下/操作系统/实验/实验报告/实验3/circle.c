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

int main(){
    while(1){
        printf("in circle\n");
        sleep(3);
    }
    return 0 ;
}