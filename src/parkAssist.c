#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("assist.log", "w");
    //char data[4];
	int rand=0;
	 if (p == NULL || log == NULL)
        {
            perror("errore in apertura file");
            exit(-1);
            /* code */
	}
	int n=0;
	while(fread(&rand,1,4,p)==4 && n<10) {
		//int c= atoi(data);
		//printf("%d",c);
		//fwrite(&rand,4,1,log);
		fprintf(log, "%d",rand);

	//prova con 10 termini
		n=n+1;
	//sleep(1); 

   }
   fclose(log);
   fclose(p);
    
    //exit(0);
}
