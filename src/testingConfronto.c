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
	int data[1];//= {0x134D, 0xFE43, 0x172A};
	FILE *p = fopen("input/urandomARTIFICIALE.binary", "r");
	fread(data, 1, 1, p);

	unsigned int conf[] = {0x172A, 0xD693, 0x0, 0xBDD8, 0xFAEE, 0x4300};
	for (int j = 0; j < 6; j++)
	{
		char c = (char)conf[j];
		if (strstr(data, conf[j]) != NULL)
		{
			printf("CIAO\n");
			return 0;
		}
	}
	return 0;
}
