#pragma once
#include "jrombk.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT 8080

typedef struct ServerConnection {
   int socketFd;
   int clientSocket1;
   int clientSocket2;
   struct sockaddr_in address;
   socklen_t addressSize;
   int valRead;
   int bufferIn;
} ServerConnection;

typedef struct ClientConnection {
    int socketFd;
    int clientFd;
    struct sockaddr_in address;
    socklen_t addressSize;
} ClientConnection;

void initServer(ServerConnection* server, int port);

int getClientData(int socketFd);

void sendClientData(int socketFd, char buffer[], int size);

void closeServer(ServerConnection* server);

void initClient(ClientConnection* client, char* address, int port);

void getServerData(ClientConnection* client, char buffer[], int size);

void sendServerData(ClientConnection* client, int val);

void closeClient(ClientConnection* client);

