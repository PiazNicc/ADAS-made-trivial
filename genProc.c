#include <stdio.h>
#include <unistd.h>

/*Dovrebbe funzionare la creazione dei processi, getegid() DOVREBBE resistuire l'id del gruppo di appartenenza (su internet c'è poca documentazione quindi non ne sono sicuro),può essere utile per uccidere tutti i processi,tuttavia torna sempre 1000 (Non so se è normale)
Altra insicurezza: le wait() per evitare che i processi muoiano prima del termine del programma, in questo modo SEMBRA che funzioni*/

int  main(int argc, char *argv[]){
	printf("Processo input,il mio pid è : %d\n",getpid());
	int pidECU_S=fork();
	if(pidECU_S == 0){  //ECU SERVER CREATO
		printf("Processo ECU Server,il mio pid è : %d , sono figlio di: %d e il mio pid gruppo è: %d\n",getpid(),getppid(),getegid());
		int pidECU_C = fork();
		if(pidECU_C == 0){	//ECU CLIENT
			printf("Processo ECU Client,il mio pid è : %d , sono figlio di: %d e il mio pid gruppo è: %d\n",getpid(),getppid(),getegid());
		}else{		//Server crea un altro figlio -throttle
			int pidThrottle = fork();
			if(pidThrottle == 0){    // THROTTLE
				printf("Processo Acceleratore,il mio pid è : %d , sono figlio di: %d e il mio pid gruppo è: %d\n",getpid(),getppid(),getegid());

			}else{
				int pidSteer = fork();	//Server crea un altro figlio- STEER
				if(pidSteer ==0){		//STEER
					printf("Processo Sterzo,il mio pid è : %d , sono figlio di: %d e il mio pid gruppo è: %d\n",getpid(),getppid(),getegid());

				}else{		
					int pidBrake = fork();		//Server crea un altro figlio -Brake
					if(pidBrake == 0){		//BRAKE
						printf("Processo Freno,il mio pid è : %d, sono figlio di: %d e il mio pid gruppo è: %d \n",getpid(),getppid(),getegid());

					}else{
						int pidWind = fork(); //Server crea un altro figlio -Windshield
						if(pidWind == 0){		//WINDSHIELD
							printf("Processo Windshield,il mio pid è : %d , sono figlio di: %d e il mio pid gruppo è: %d \n",getpid(),getppid(),getegid());

						}else{
							int pidForward = fork();	//Server crea un altro figlio -Forward
							if(pidForward == 0){		//FORWARD
								printf("Processo ForwardCamera,il mio pid è : %d , sono figlio di: %d\n",getpid(),getppid(),getegid());

							}else{
								int pidBlindSpot = fork();	//Server crea un altro figlio-BlindSpot
								if(pidForward == 0){		//BLINDSPOT
									printf("Processo BlindSpot, il mio pid è: %d, sono figlio di: %d e il mio pid gruppo è: %d\n",getpid(),getppid(),getegid());				

								}else{
									wait();
								}
							}wait();				
						}wait();
					}wait();
				}wait();
			}wait();	
		}wait();
	}wait();
}	
