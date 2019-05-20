#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("assist.log", "w");
    char data[4];
	 if (p == NULL || log == NULL)
        {
            perror("errore in apertura file");
            exit(-1);
            /* code */
	}
	int n = 0;
      while(fread(data,1,4,p)!=NULL && n<10) {
     	
	puts(data);
	//printf("%d",sizeof(data));
         fwrite(data, sizeof(char),sizeof(data),log);
	 n=n+1;
	//prova con 10 termini

	//sleep(1); 

   }
   fclose(log);
   fclose(p);
    
    //exit(0);
}
