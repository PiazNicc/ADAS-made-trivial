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
	unsigned char data[] = {0x134D, 0xFE43, 0x172A};
	FILE *p = fopen("/dev/random", "r");
	//fread(data, 1, 24, p);

	unsigned char conf[] = {0x172A, 0xD693, 0x0, 0xBDD8, 0xFAEE, 0x4300};
	for (int i = 0; i < sizeof data / sizeof data[0]; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (data[i] == conf[j])
			{
				printf("ciao\n");
			}
		}
	}
	return 0;
}
