#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "SocketConnection.h"
#include "sensori.h"
void frontWindshield()
{
    {
        FILE *wscIN = fopen("input/frontCamera.data", "r");
        FILE *wscOUT = fopen("log/camera.log", "a");
        char data[255];
        int ecu;

        if (wscIN == NULL)
        {
            perror("error not open fileINPUT");
            exit(EXIT_FAILURE);
        }
        if (wscOUT == NULL)
        {
            perror("error not open fileOUTPUT");
            exit(EXIT_FAILURE);
        }
        while (fgets(data, sizeof(data), wscIN) != NULL)
        {

            ecu = connectToServer(".ecu");
            send(ecu, data, sizeof(data), 0);
            close(ecu);
            fputs(data, wscOUT);
            fflush(wscOUT);
            sleep(10);
        }

        fclose(wscOUT);
        fclose(wscIN);
    }
}
void parkAssist(int mode)
{
    FILE *p;
    int data[4];
    int secs = 0, i;
    if (mode == 0)
    {
        p = fopen("/dev/urandom", "r");
    } else
    {
        p = fopen("input/urandomARTIFICIALE.binary","r");
    }
    

    FILE *log = fopen("log/assist.log", "w");
    int ecuServer;
    if (p == NULL || log == NULL)
    {
        perror("errore in apertura file");
        exit(EXIT_FAILURE);
    }
    for (secs = 0; secs < 30; secs++)
    {
        //
        /*if (fread(data, 1, 4, p) < 4)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }*/
        for (i = 0; i < 4; i++)
        {
            data[i] = getc(p);
            fprintf(log, "%0x ", data[i]);
        }

        // ecuServer = connectToServer("ecu");
        ///if (send(ecuServer, data, strlen(data), 0) < 0)
        //{
        ///    perror("send");
        // /    exit(EXIT_FAILURE);
        // }
        fprintf(log, "\n");
        fflush(log);
        sleep(1);
    }
    fclose(log);
    fclose(p);
}
void forwardFacing(int mode)
{
    char data[24];
    FILE *p = fopen("/dev/random", "r");
    FILE *log = fopen("log/radar.log", "a");
    int ecuServer;
    for (;;)
    {
        if (fread(data, 1, 24, p) == 24)
        {
            ecuServer = connectToServer("ecu");
            if (send(ecuServer, data, strlen(data), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            if (fwrite(data, 1, 4, log) < 4)
            {
                perror("write");
                exit(EXIT_FAILURE);
            };
            close(ecuServer);
            fprintf(log, "\n");
            fflush(log);
        }
        sleep(2);
    }
}
void surroundViews(int mode)
{
    char data[16];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("log/cameras.log", "a");
    int ecuServer;
    for (int secs = 0; secs < 30; secs++)
    {
        //
        if (fread(data, 1, 16, p) < 16)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (fwrite(data, 1, 16, log) < 16)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        ecuServer = connectToServer("ecu");
        if (send(ecuServer, data, strlen(data), 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        fprintf(log, "\n");
        sleep(1);
    }
    fclose(log);
    fclose(p);
}

void blindSpot(int mode)
{
    char data[8];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("log/spot.log", "a");
    int ecuServer;
    for (int secs = 0; secs < 4; secs++)
    {
        //
        if (fread(data, 1, 8, p) < 8)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (fwrite(data, 1, 8, log) < 8)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        ecuServer = connectToServer("ecu");
        if (send(ecuServer, data, strlen(data), 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        fprintf(log, "\n");
        sleep(1);
    }
    fclose(log);
    fclose(p);
}