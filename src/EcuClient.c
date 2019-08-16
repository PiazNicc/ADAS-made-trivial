#include <stdio.h>
#include <stdlib.h>
#include "SocketConnection.h"
#define READ 0
#define WRITE 1
#define NComponenti 3


int main(int fd[])
{
		char connectionName[NComponenti][10]={{"steer"},{"brake"},{"throttle"}};
		int vel=0;
		int bytesRead;
		char message[100];
		int clientFD[NComponenti];
		close(fd[WRITE]);
		int i=0;
		while(i<NComponenti)
		{
			clientFD[i]=connectToServer(connectionName[i]);
			if(clientFD[i]>0)i++;
			else{fprintf(stderr, "impossibile connettersi a %s \n",connectionName[i]);}
			/*switch i{
			case 0:
			{
				clientFD[i]=connectToServer("steer");
				break;}
			case 1:
			{
				clientFDconnectToServer("brake");
				break;}
			case 2:
			{
				connectToServer("throttle");
				break;}
			case 3:
			{
				connectToServer();
				break;}*/	
		}
	while(1)
	{
		bytesRead = read (fd[READ], message, 100);
		int fd;
		char *line1;
		int result;
		int start=1;
		if(bytesRead>0)
		{
			if(strcmp(message,"INIZIO"))
			{
				start=1;
			}
			else{
				if(start==1)
				{
				if(strcmp(message,"DESTRA")||strcmp(message,"SINISTRA"))
				{
					fd=0;
					line1=message;
				}
				else{
					if(strcmp(message,"PERICOLO"))
					{
						vel=0;
						start=0;
						//signal pericolo;
					}
					else{
						if(strcmp(message,"PARCHEGGIO"))
						{
			
						}
						else{
						int n=0;
						for(int i=0;i<bytesRead;i++)
						{
							n=(n*10)+((int)message+1)-49;}
						if(n>vel)
						{
							fd=2;
							line1= strcat("INCREMENTO ",(char)(n-vel));
							vel=n;
						}
						else{
							fd=1;
							line1= strcat("FRENO ",(char)(n-vel));
							vel=n;
						}
						
												
						}
				}}
			}
		}}
	result=writeToSocket(fd,line1);
	}


}
