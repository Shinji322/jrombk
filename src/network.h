#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 3490
#define MAXSIZE 1024
#define BACKLOG 2

#include "jrombk.h"


typedef struct ServerConnection {
    struct sockaddr_in server;
    struct sockaddr_in dest;
    int status, socket_fd, client1_fd, client2_fd, num;
    socklen_t size;

    char buffer[10241];
    char *buff;
} ServerConnection;

typedef struct ClientConnection {
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;
    char buffer[1024];
    //char bufferIn[MAP_HEIGHT * MAP_WIDTH * 2];
} ClientConnection;

void initServer(ServerConnection* main);

int networkGetch(ServerConnection* main, bool isPlayerOne);

void sendServerData(ServerConnection* main, bool isPlayerOne);

void closeServer(ServerConnection* main);

void initClient(ClientConnection* main, char* address);

void clientPut(ClientConnection* main, int data);

void receiveServerData(ClientConnection* main);

void closeClient(ClientConnection* main);
