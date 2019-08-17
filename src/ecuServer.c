#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include "SocketConnection.h"
#include "Attuatori.h"
#define READ 0
#define WRITE 1
#define MaxConnection 5
void killProc(int array[])
{
    for (int i = 0; i < 7; i++)
    {
        printf("Processo[%d]: %d---\n", i, array[i]);
        kill(array[i], SIGKILL);
        wait();
    }
}
int main()
{
    /* versione base che gestisce solo un sensore(front windshield),per i sensori
    facoltativi andrebbe implementata la gestione in contemporanea di 
    piu client
    */
    char *command;
    int speed = 0, serverD, clientD[5],len;
    struct sockaddr_un client;
    printf("Processo ECU Server,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
    printf("ID del gruppo è: %d\n", getpgid(getpid()));

    int pidWindshield = fork();
    int pidArray[7];
    int i = 0;
    if(pidWindshield < 0){perror("fork"); exit(EXIT_FAILURE);}
    if (pidWindshield == 0)
    { //windshield
        /*CODICE del processo*/
        printf("Processo frontWindshield,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
        sleep(100);
    }
    else
    { //Server crea un altro figlio -forward radar
        pidArray[i] = pidWindshield;
        i++;
        int pidRadar = fork();
        if(pidRadar < 0){perror("fork"); exit(EXIT_FAILURE);}
        if (pidRadar == 0)
        { // radar
            /*CODICE del processo radar*/
            printf("Processo radar,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
            sleep(100);
        }
        else
        {
            pidArray[i] = pidRadar;
            i++;

            int pidSteer = fork();
            if(pidSteer < 0){perror("fork"); exit(EXIT_FAILURE);} //Server crea un altro figlio- STEER
            if (pidSteer == 0)
            { //STEER
                /*CODICE del processo Steer*/ steerByWire();
                printf("Processo Sterzo,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
                sleep(100);
            }
            else
            {
                pidArray[i] = pidSteer;
                i++;
                int pidBrake = fork(); 
                if(pidBrake < 0){perror("fork"); exit(EXIT_FAILURE);}//Server crea un altro figlio -Brake
                if (pidBrake == 0)
                { //BRAKE
                    brakeByWire();
                    printf("Processo Freno,il mio pid è : %d, sono figlio di: %d \n", getpid(), getppid());
                    sleep(100);
                }
                else
                {
                    pidArray[i] = pidBrake;
                    i++;

                    int pidThrottle = fork();
                    if(pidThrottle< 0){perror("fork"); exit(EXIT_FAILURE);} //Server crea un altro figlio -throttle
                    if (pidThrottle == 0)
                    { //
                        throttleControl();
                        /*CODICE del processo, ho usato sleep per testare la kill*/
                        printf("Processo throttle,il mio pid è : %d , sono figlio di: %d\n", getpid(), getppid());
                        sleep(100);
                    }
                    else
                    {
                        pidArray[i] = pidThrottle;
                        i++;

                        int pidClient = fork(); 
                        if(pidClient < 0){perror("fork"); exit(EXIT_FAILURE);}//Server crea un altro figlio -client
                        if (pidClient == 0)
                        { //client
                            /*CODICE del processo*/
                            printf("Processo ecuClient,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
                            sleep(100);
                        }
                        else
                        {
                            pidArray[i] = pidClient;
                            i++;
                            killProc(pidArray);
                            len = sizeof(client);
                            serverD = serverSocket("ecu");
                            listen(serverD, MaxConnection);
		      int i=0;
                            while (i<MaxConnection)
                            {
                                clientD[i] = accept(serverD, (struct sockaddr *)&client, &len);
                                if (clientD[i] < 0)
                                {
                                    perror("accept");
                                    exit(EXIT_FAILURE);
                                }
			else
			{
			i++;
			}
		      }
			int fd[2];
			pipe(fd);
			if(fork()==0)
			{
                close(fd[WRITE]);
				ecuclient(fd);
                                //INVIA COMANDO A ECUCLIENT(pipe?)
                                //logga comando in ecu.log(oppure lo fa ecuClient?)
			}
			close(fd[READ]);
			i=0;
			while(1)
			{
                                if (recv(clientD[i], command, strlen(command), 0) < 0)
                                {
                                    perror("receive");
                                    exit(EXIT_FAILURE);
                                }
			else i=(i+1)%MaxConnection;
			write (fd[WRITE], command, strlen (command) + 1);
			
                            }

                        } //chiusura else forward

                    } //chiusura else windishield

                } // chiusura else brake

            } //chiusura else steer

        } //chiusura else throttle
    }
    //chiusura else ecu client
}
