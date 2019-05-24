#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/*SEMBRA funzionare, ho messo sleep 100 per evitare che muoiano subito i processi, in modo da poter provare la kill*/


int  main(int argc, char *argv[]){
	printf("Processo input,il mio pid è : %d\n",getpid());
	int pidECU_S=fork();
	if(pidECU_S == 0){  //ECU SERVER CREATO
		printf("Processo ECU Server,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
		int pidECU_C = fork();
		int pidArray[7];
		int i=0;
		if(pidECU_C == 0){	//ECU CLIENT
		/*CODICE del processo, ho usato sleep per testare la kill*/	
			printf("Processo ECU Client,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
			sleep(100);
			
		}else{		//Server crea un altro figlio -throttle
			pidArray[i] = pidECU_C;
			i++;
			int pidThrottle = fork();
			if(pidThrottle == 0){    // THROTTLE
				/*CODICE del processo, ho usato sleep per testare la kill*/
				printf("Processo Acceleratore,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
				sleep(100);

			}else{
				pidArray[i] = pidThrottle;
				i++;

				int pidSteer = fork();	//Server crea un altro figlio- STEER
				if(pidSteer ==0){		//STEER
					/*CODICE del processo, ho usato sleep per testare la kill*/
					printf("Processo Sterzo,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());sleep(100);
				}else{		
					pidArray[i] = pidSteer;
					i++;
					int pidBrake = fork();		//Server crea un altro figlio -Brake
					if(pidBrake == 0){		//BRAKE
						printf("Processo Freno,il mio pid è : %d, sono figlio di: %d \n",getpid(),getppid());
						sleep(100);

					}else{
						pidArray[i] = pidBrake;
						i++;
						
						
						int pidWind = fork(); //Server crea un altro figlio -Windshield
						if(pidWind == 0){		//WINDSHIELD
						/*CODICE del processo, ho usato sleep per testare la kill*/
							printf("Processo Windshield,il mio pid è : %d , sono figlio di: %d\n",getpid(),getppid());
							sleep(100);

						}else{
							pidArray[i] = pidWind;
							i++;
							
							int pidForward = fork();	//Server crea un altro figlio -Forward
							if(pidForward == 0){		//FORWARD
							/*CODICE del processo, ho usato sleep per testare la kill*/
								printf("Processo ForwardCamera,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
								sleep(100);
								
							}else{
								pidArray[i] = pidForward;
								i++;
								int pidBlindSpot = fork();	//Server crea un altro figlio-BlindSpot
								if(pidBlindSpot == 0){		//BLINDSPOT
							/*CODICE del processo, ho usato sleep per testare la kill*/
									printf("Processo BlindSpot, il mio pid è: %d, sono figlio di: %d \n",getpid(),getppid());
									sleep(100);					
								}else{
									pidArray[i] = pidBlindSpot;
									/*Non sono sicuro che si possa fare qui, ma sembra funzionare*/
									for(int j=0;j<7;j++){
										kill(pidArray[j],SIGKILL);
									}
								}wait();
							}wait();				
						}wait();
					}wait();
				}wait();
			}wait();	
		}wait();
	
	}wait();sleep(100);

}
