#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int forwardFacing(unsigned char *data)
{
    
    FILE *p = fopen("/dev/random", "r");
    return fread(data,1, 24, p);

     

}

int check(char *data,int n){
	unsigned int conf[]={0xA00F,0xB072,0x2FA8,0x8359,0xCE23};
	unsigned char input[2];
	size_t num = sizeof(conf)/sizeof(conf[0]);
	printf("starting now\n");
	for(int i=0;i<n-1;i++){
		input[0]=data[i];				//ogni tanto ci finiscono valori strani
		printf("the data 0 is:%X\n",data[0]);
		input[1]=data[i+1];
		printf("the data 1 is:%X\n",data[i+1]);
		printf("cicle n:%d\n",i);
		for(int j=0;j<num;j++){
			printf("confront cicle n:%d,the conf is %X:,the input is:%s\n",j,conf[j],input);
			
			if(memcmp(input,&conf[j],2)==0){		
				printf("success");
				return 0;
			}
		}	
	}
}

void main(){
	unsigned char data[24];
	int n= forwardFacing(data);
	check(data,n);

}
