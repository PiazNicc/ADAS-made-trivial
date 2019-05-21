#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	FILE *p = fopen("/dev/urandom", "r");
	FILE *log = fopen("assist.log", "w");
	char data[4];
	int rng = 0;
	if (p == NULL || log == NULL)
	{
		perror("errore in apertura file");

		/* code */
	}
	for (int i = 0; i < 4; i++)
	{
		rng = getc(p);/* code */
        fprintf(log,"%d ",rng);
	}
	
	
	fclose(log);
	fclose(p);

	//exit(0);
}
