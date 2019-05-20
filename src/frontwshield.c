#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
 
FILE *openFileInput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("frontCamera.data","rb");

}

FILE *openFileOutput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("camera.log","w");
}



void readData(FILE *fin,FILE *fout){
	char buffer[15];
	while(fgets(buffer,sizeof(buffer),fin)!=NULL){
		/*codice per inviare il dato*/
		writeData(fout,buffer);
		//sleep(10);
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

	
	if(wscIN==NULL){
	printf("error not open fileINPUT");
	error++;
	}
	if(wscOUT==NULL){
	printf("error not open fileOUTPUT");
	error++;
	}
	if(error==0){
	readData(wscIN,wscOUT);
	
	}
	fclose(wscOUT);
	fclose(wscIN);
	
}



