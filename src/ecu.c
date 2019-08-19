#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "SocketConnection.h"
#include "ecu.h"
#include "attuatori.h"
#include "sensori.h"
#define READ 0
#define WRITE 1
void killCom(int sig)
{
    kill(0, SIGKILL);
    exit(0);
}
void ecu()
{
    /* versione base che gestisce solo un sensore(front windshield),per i sensori
    facoltativi andrebbe implementata la gestione in contemporanea di 
    piu client
    */
    char in[128], command[128];
    memset(command, 0, sizeof(command));
    struct sockaddr_un client;
    int speed = 0, serverD, clientD;
    unsigned int len = sizeof(client);

    printf("Processo ECU Server,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
    printf("ID del gruppo è: %d\n", getpgid(getpid()));

    int pidWindshield = fork();
    int pidArray[5];
    int i = 0;
    if (pidWindshield < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pidWindshield == 0)
    { //windshield
        frontWindshield();
        // printf("Processo frontWindshield,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
        // sleep(100);
    }
    else
    { //Server crea un altro figlio -forward radar
        pidArray[i] = pidWindshield;
        i++;
        int pidRadar = fork();
        if (pidRadar < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pidRadar == 0)
        { // radar
            /*CODICE del processo radar*/
            //printf("Processo radar,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
            //sleep(100);
        }
        else
        {
            pidArray[i] = pidRadar;
            i++;

            int pidSteer = fork();
            if (pidSteer < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            } //Server crea un altro figlio- STEER
            if (pidSteer == 0)
            { //STEER
                /*CODICE del processo Steer*/ steerByWire();
                /// printf("Processo Sterzo,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
                //sleep(100);
            }
            else
            {
                pidArray[i] = pidSteer;
                i++;
                int pidBrake = fork();
                if (pidBrake < 0)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                } //Server crea un altro figlio -Brake
                if (pidBrake == 0)
                { //BRAKE
                    brakeByWire();
                    //printf("Processo Freno,il mio pid è : %d, sono figlio di: %d \n", getpid(), getppid());
                    //sleep(100);
                }
                else
                {
                    pidArray[i] = pidBrake;
                    i++;

                    int pidThrottle = fork();
                    if (pidThrottle < 0)
                    {
                        perror("fork");
                        exit(EXIT_FAILURE);
                    } //Server crea un altro figlio -throttle
                    if (pidThrottle == 0)
                    { //
                        throttleControl();
                        /*CODICE del processo, ho usato sleep per testare la kill*/
                        /// printf("Processo throttle,il mio pid è : %d , sono figlio di: %d\n", getpid(), getppid());
                        // sleep(100);
                    }
                    else
                    {
                        pidArray[i] = pidThrottle;
                        i++;
                        int fd[2];
                        pipe(fd);
                        int pidClient = fork();
                        if (pidClient < 0)
                        {
                            perror("fork");
                            exit(EXIT_FAILURE);
                        } //Server crea un altro figlio -client
                        else if (pidClient == 0)
                        { //client
                            /*CODICE del processo*/
                            close(fd[WRITE]);
                            // ecuClient(fd);
                            //  printf("Processo ecuClient,il mio pid è : %d , sono figlio di: %d \n", getpid(), getppid());
                            // sleep(100);
                        }
                        else
                        {
                            signal(SIGKILL, SIG_IGN);
                            pidArray[i] = pidClient;
                            close(fd[READ]);
                            serverD = serverSocket("ecu");
                            listen(serverD, 5);
                            while (strcmp(command, "PARCHEGGIO") != 0)
                            {
                                clientD = accept(serverD, (struct sockaddr *)&client, &len);

                                if (clientD < 0)
                                {
                                    fprintf(stderr, "impossibile connettersi");
                                }
                                if (recv(clientD, command, sizeof(command), 0) < 0)
                                {
                                    fprintf(stderr, "impossibile leggere");
                                }
                                printf("%s", command);
                                close(clientD);
                            }
                            kill(0,SIGKILL);
                            int park = fork();
                            if (park < 0)
                            {
                                exit(-1);
                            }
                            else if (park == 0)
                            {
                                parkAssist();
                            }
                            else
                            {
                                printf("parcheggio\n");
                            }

                        } //chiusura else forward

                    } //chiusura else windishield

                } // chiusura else brake

            } //chiusura else steer

        } //chiusura else throttle
    }
}

void ecuClient(int pipe[])
{
    char message[128];
    memset(message, 0, sizeof(message) + 1);
    while (1)
    {
        if (read(pipe[READ], message, strlen(message) + 1) > 0)
        {
                }
        sleep(1);
        /* code */
    }
}