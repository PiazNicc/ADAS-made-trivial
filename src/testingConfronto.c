#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int forwardFacing(unsigned char *data)
{

	FILE *p = fopen("/dev/random", "r");
	return fread(data, 1, 24, p);
}

void main()
{
	unsigned char data[24];
	FILE *p = fopen("/dev/random", "r");
	fread(data, 1, 24, p);
	unsigned char conf[] = {0xA00F, 0xB072, 0x2FA8, 0x8359, 0xCE23};
	for (int i = 0; i < sizeof data / sizeof data[0]; i++)
	{
		char *f = memchr(conf, data[i], sizeof(conf));
		if (f != NULL)
		{
			printf("ciao\n");
		}
	}
	return 0;
}
