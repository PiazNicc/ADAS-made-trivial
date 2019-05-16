#include<stdio.h>
#include<string.h>

typedef struct String {
char data[10];
int length;}string;

 
FILE *openFileInput(){
FILE *wscPunt;
return wscPunt=fopen(" frontCamera.data","R");
}
FILE *openFileOutput(){
FILE *wscPunt;
return wscPunt=fopen("camera.log","w");
}

string readData(FILE *Punt)
{
	string Value;
	char oneData[2];
	Value.length=0;	
	do{	
	fread(oneData,1,1,Punt);
	Value.data[Value.length]=oneData[1];	
	Value.length++;
	
	printf("%s",oneData);
	}
	while(strcmp(oneData,"\n"));
	return Value;
}
void writeData(FILE *Punt,char *data)
{
	fwrite(data,1,sizeof(data),Punt);


}

void main()
{
	FILE *wscIN=openFileInput();
	FILE *wscOUT=openFileOutput();
	string data;
	data=readData(wscIN);
	//writeData(wscOUT,data);
	
}


