#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *forwardFacing()
{
    char data[24];
    FILE *p = fopen("/dev/random", "r");
    fread(data,1, 24, p);

    return data;

}

int check(char *data){
	unsigned char conf[11]={0xA00F,0xB072,0x2FA8,0x8359,0xCE23};  //non ho idea se si possa inizializzare cosi,2byte x5 + terminatore = 11
	char input[2];

	for(int i=0;i<strlen(data)-1;i++){
		input[0]=data[i];
		input[1]=data[i+1];
		for(int j=0;j<strlen(conf)-2;j+2){
			if(memcmp(input,conf[j],2)==0){
				printf("success");
				return 0;
			}
		}	
	}
}

void main(){

	check(forwardFacing());

}
