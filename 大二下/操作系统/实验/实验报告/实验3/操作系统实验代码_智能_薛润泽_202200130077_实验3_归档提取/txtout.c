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
    char c[100];
    char temp = getchar();
    int cnt = 0;
    while(temp!='\n'){
        c[cnt] = temp;
        temp = getchar(); cnt++;
    }
    c[cnt] = '\0';
    printf("%s\n",c);
}