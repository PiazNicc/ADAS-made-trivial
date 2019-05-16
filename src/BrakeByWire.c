#include <stdio.h>
#include<fcntl.h>
#include<string.h>
#include <unistd.h>

void decreaseSpeed(int amount,FILE *f){
        int len = strlen("DECREMENTO 5\n");
	while(amount !=0){
		fwrite("DECREMENTO 5\n",1,len,f);
		amount = amount-5;
		sleep(1);
	}
}

void stop(FILE *f){
	int len = strlen("ARRESTO AUTO\n");
	fwrite("ARRESTO AUTO\n",1,len,f);
	sleep(1);
}

void noAction(FILE *f){
	int len = strlen("NO ACTION\n");
	fwrite("NO ACTION\n",1,len,f);
	sleep(1);
}

int findSubString(char str[],int pos){
	int lengthOfSub= sizeof(str)-pos;
	int amount =0;
	char subStr[lengthOfSub];
	int i=0;
	while(i<lengthOfSub){
		subStr[i] = str[pos+i];
		i++;								
	}
	return amount = atoi(subStr);
	
}

int main(int argc, char *argv[]){
	FILE * f;
	char str[10];
	f = fopen("brake.log","a");
	int decAmount;
	printf("Inserisci azione: ");
	fgets(str,15,stdin);
	//printf("%s",str);
	int n = strcmp(str,"ARRESTO");
	if(strcmp(str,"ARRESTO")==10){
		stop(f);
	}else if(strcmp(str,"NO ACTION")==10){
		noAction(f);
	}else{
		decAmount = findSubString(str,6);
		decreaseSpeed(decAmount,f);
	}
	
	fclose(f);	
}
