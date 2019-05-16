#include<stdio.h>
#include <unistd.h>



FILE *openFileOutput(){
FILE *steerPunt;
return steerPunt=fopen("steer.log","w");
}

void Scrivi(int i,FILE *steerOu)
{

	char *ma[]={"STO GIRANDO A DESTRA","STO GIRANDO A SINISTRA","NO ACTION"};
	int maLenght[]={20,22,9};

	
	int k;
	if(i!=2)
	{
	for(int j=0;j<4;j++)
	{
	
	k=fwrite(ma[i],1,maLenght[i],steerOu);
	fwrite("\n",1,1,steerOu);
	sleep(1);
	}
	}
	else
	{
	k=fwrite(ma[i],1,maLenght[i],steerOu);
	fwrite("\n",1,1,steerOu);
	}
}

void main()
{
	FILE *steerOu=openFileOutput();
	
	Scrivi(2,steerOu);
	Scrivi(0,steerOu);
	Scrivi(1,steerOu);
}



