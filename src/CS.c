#include "ClientConnectionManagement.h"
#include<stdio.h>


void main()
{

	
	int clientFD=createSocket("pip");
	char t[200];
	rClient (clientFD,t);
	printf("%s",t);
	close_CS(clientFD);
}
	
