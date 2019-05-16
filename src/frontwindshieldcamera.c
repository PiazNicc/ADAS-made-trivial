  
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
	fread(&oneData,1,1,Punt);
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
	FILE *wscIN=openFileInput();
	FILE *wscOUT=openFileOutput();
	string data;
	if(wscIN==null){
	printf("error not open fileINPUT");}
	else{
	 data=readData(wscIN);}
	//invio file al server;
	if(wscOUT==null){
	printf("error not open fileOUTPUT");}
	else
	writeData(wscOUT,data);
	sleep(10);
	

}



