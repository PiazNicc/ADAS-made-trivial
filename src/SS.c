#include"ServerOneConnection.h"
#include<stdio.h>
#include <unistd.h>

void main()
{
	
	char t[200];
	int serverFD=make_connection("pip");
	int clientFD=make_socket(serverFD);
	wServer(clientFD,"ciao come va?");
	close_SS(clientFD);
}
