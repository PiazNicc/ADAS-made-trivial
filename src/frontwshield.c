#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include "SocketConnection.h"


FILE *openFileInput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("input/frontCamera.data","r");

}

FILE *openFileOutput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("camera.log","w");
}



void readData(FILE *fin,FILE *fout,int clientFD){
	char buffer[15];
	while(fgets(buffer,sizeof(buffer),fin)!=NULL){
		/*codice per inviare il dato*/
		writeToSocket(clientFD,buffer);
		writeData(fout,buffer);
		sleep(10);
	}
}
void writeData(FILE *Punt,char *buffer)
{
	
	fwrite(buffer,1,strlen(buffer),Punt);  //strlen in questo caso è sicuro perchè fgets mette SEMPRE il terminatore a fine lettura
	//puts(buffer);
	

}


void main()


{
	int error=0;
	FILE *wscIN=openFileInput();
	FILE *wscOUT=openFileOutput();
	int clientFd=connectToServer("ecu");
	
	if(wscIN==NULL){
	printf("error not open fileINPUT");
	error++;
	}
	if(wscOUT==NULL){
	printf("error not open fileOUTPUT");
	error++;
	}
	if(error==0){
	readData(wscIN,wscOUT,clientFd);
	}
	fclose(wscOUT);
	fclose(wscIN);
	
}



