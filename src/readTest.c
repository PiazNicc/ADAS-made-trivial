#include <stdio.h>

void readLine(char buffer[],FILE *f,int maxChar){

   while( fgets (buffer,maxChar, f)!=NULL ) {
      	puts(buffer);
	//in qualche modo invia il data prima che inizi il prossimo ciclo
	sleep(1); 
   }
}

int main(int argc, char *argv[]){
	FILE *f;
	int maxChar = 20;
	char str[maxChar];
	f = fopen("data.log","r");

	if(f == NULL){
		printf("File not found");
		return;
	}else{
		readLine(str,f,maxChar);
	}
	
	fclose(f);
	
}
