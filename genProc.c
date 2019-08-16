#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/*Creazione dei processi e inserimento pid nell'array funziona, ma la kill non funziona correttamente,se faccio kill alcuni processi non stampano il messaggio*/

void killProc(int array[]){
	for(int i=0;i<7;i++){
		printf("Processo[%d]: %d---\n",i,array[i]);
		kill(array[i],SIGKILL);   
		wait();
	}
}

int  main(int argc, char *argv[]){
	
	
	 //ECU SERVER 
		printf("Processo ECU Server,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
		printf("ID del gruppo è: %d\n",getpgid(getpid()));

		int pidECU_C = fork();
		int pidArray[7];
		int i=0;

		if(pidECU_C == 0){	//wind
		/*CODICE del processo*/	
			printf("Processo frontWindshield,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
			sleep(100);
			
		}else{		//Server crea un altro figlio -throttle
			pidArray[i] = pidECU_C;
			i++;
			int pidThrottle = fork();
			if(pidThrottle == 0){    // THROTTLE
				/*CODICE del processo Throttle*/
				printf("Processo Acceleratore,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
				sleep(100);

			}else{
				pidArray[i] = pidThrottle;
				i++;

				int pidSteer = fork();	//Server crea un altro figlio- STEER
				if(pidSteer ==0){		//STEER
					/*CODICE del processo Steer*/
					printf("Processo Sterzo,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
					sleep(100);
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
							/*CODICE del processo*/
								printf("Processo ForwardCamera,il mio pid è : %d , sono figlio di: %d \n",getpid(),getppid());
								sleep(100);
								
							}else{
								pidArray[i] = pidForward;
								i++;
								int pidBlindSpot = fork();	//Server crea un altro figlio-BlindSpot
								if(pidBlindSpot == 0){		//BLINDSPOT
							/*CODICE del processo*/
									printf("Processo BlindSpot, il mio pid è: %d, sono figlio di: %d \n",getpid(),getppid()); 
									sleep(100);
								}else{
									pidArray[i] = pidBlindSpot;
									printf("Now ECU is gonna kill the processes\n");
							/*CODICE DEL PROCESSO ECU SERVER*/
									killProc(pidArray); //stamp il pid di tutti i processi
									while(1){}
								}

							}	//chiusura else forward
			

						} //chiusura else windishield


					}	// chiusura else brake

				
				}		//chiusura else steer


			}			//chiusura else throttle
	

		}
				//chiusura else ecu client
	
	}


