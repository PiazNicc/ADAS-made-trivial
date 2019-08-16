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
void frontWindshield() {}
void parkAssist()
{
    unsigned char data[4];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("assist.log", "w");
    int len = sizeof(data), ecuServer;
    unsigned int rng = 0;
    if (p == NULL || log == NULL)
    {
        perror("errore in apertura file");
        exit(EXIT_FAILURE);
    }
    for (int secs = 0; secs < 30; secs++)
    {
        //
        if (fread(data, 1, 4, p) < 4)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (fwrite(data, 1, 4, log) < 4)
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
        fprintf("\n", log);
        sleep(1);
    }
    fclose(log);
    fclose(p);
}
void forwardFacing()
{
    unsigned char data[24];
    FILE *p = fopen("/dev/random", "r");
    FILE *log = fopen("radar.log", "a");
    int len = sizeof(data), ecuServer;
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
            fflush(log);
        }
        sleep(2);
    }
}
void surroundViews()
{
    unsigned char data[16];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("cameras.log", "a");
    int len = sizeof(data), ecuServer;
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
        fprintf("\n", log);
        sleep(1);
    }
    fclose(log);
    fclose(p);
}

void blindSpot() {
     unsigned char data[8];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("spot.log", "a");
    int len = sizeof(data), ecuServer;
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
        fprintf("\n", log);
        sleep(0.5);
    }
    fclose(log);
    fclose(p);
    
}