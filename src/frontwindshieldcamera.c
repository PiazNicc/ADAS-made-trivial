#include<stdio.h>
#include<string.h>

typedef struct String {
char data[10];
int length;
}string;

 
FILE *openFileInput(){
FILE *wscPunt;
wscPunt=0;
return wscPunt=fopen("frontCamera.data","r");

}
FILE *openFileOutput(){
FILE *wscPunt;
wscPunt=0;
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
	for(int i=0;i<100;i++){
	if(wscIN==0){
	printf("error");}
	else{
	string data=readData(wscIN);
	if(wscOUT==0){
	printf("error");}
	else
	writeData(wscOUT,data);
	}}
}


