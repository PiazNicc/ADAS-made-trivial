#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc,char *argv[]){
    printf("macchina accesa,scrivere INIZIO per mettere in moto\n\n");
    printf("quando si vuole parcheggiare,scrivere PARCHEGGIO sul terminale.\n");
    char input[10];
    scanf("%s",&input);
    while(strcmp(input,"PARCHEGGIO") !=0 ){
         scanf("%s",&input);
    }
    exit(0);
}