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
int main(){
    char c[100];
    char temp = getchar();
    int cnt = 0;
    char *command[100]; Getmalloc(command);
    while(temp!='\n'){
        c[cnt] = temp;
        temp = getchar(); cnt++;
    }
    c[cnt] = '\0';
    char sbreak = ' ';
    Decom(c, command,sbreak);
    if(execvp(command[0], command) == -1)perror("execvp");
    for(int i = 0 ; i < 100; i++)free(command[i]);
    return 0;
}