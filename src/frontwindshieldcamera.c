  
#include<stdio.h>
#include <unistd.h>
typedef struct String {
char data[10];
int length;
}string;

 
FILE *openFileInput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("frontCamera.data","r");

}
FILE *openFileOutput(){
FILE *wscPunt;
wscPunt;
return wscPunt=fopen("camera.log","w");
}

string readData(FILE *Punt)
{
	string Value;
	char oneData='0';
	Value.length=0;
	while(!(oneData=='\n')){	
	if(fread(&oneData,1,1,Punt)==0){
	printf("fine file");
	return Value;}
	Value.data[Value.length]=oneData;	
	Value.length++;
	}
	Value.length=Value.length-2;
	return Value;
}
void writeData(FILE *Punt,string data)
{
	
	fwrite(&(data.data),1,data.length+1,Punt);
	fwrite("\n",1,1,Punt);

}


void main()
{
	int error=0;
	FILE *wscIN=openFileInput();
	FILE *wscOUT=openFileOutput();
	string data;
	
	if(wscIN==NULL){
	printf("error not open fileINPUT");
	error++;
	}
	if(wscOUT==NULL){
	printf("error not open fileOUTPUT");
	error++;
	}
	if(error==0){
	do{
	 data=readData(wscIN);
	printf("%d",data.length);
	if(data.length>0)
	{
	//invio file al server;
	writeData(wscOUT,data);
	//sleep(10);
	}
	}while(data.length>0);
	}
	
	fclose(wscOUT);
	fclose(wscIN);
}



