#include "network.h"
#include <netdb.h>
#include <sys/socket.h>


void initServer(ServerConnection* server, int port) {
    int opt = 1;

    server->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socketFd < 0) {
        internalLog("Failed to init socket.");
        exit(0);
    }

    int ret = setsockopt(server->socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    if (ret) {
        internalLog("Failed to set socket opt");
        exit(0);
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);

    server->addressSize = sizeof(server->address);

    ret = bind(server->socketFd, (struct sockaddr*)&server->address, server->addressSize);

    if (ret < 0) {
        internalLog("Bind error");
        exit(0);
    }

    ret = listen(server->socketFd, 3);

    if (ret < 0) {
        internalLog("Listen error");
        exit(0);
    }


    printf("Waiting for Player 1 to connect\n");
    server->clientSocket1 = accept(server->socketFd, (struct sockaddr*)&server->address, &server->addressSize);

    if (server->clientSocket1 < 0) {
        internalLog("Player 1 accept error");
        exit(0);
    }

    
    printf("Waiting for Player 2 to connect\n");
    server->clientSocket2 = accept(server->socketFd, (struct sockaddr*)&server->address, &server->addressSize);

    if (server->clientSocket2 < 0) {
        internalLog("Player 2 accept error");
        exit(0);
    }

    fcntl(server->clientSocket1, F_SETFL, FNDELAY);
    fcntl(server->clientSocket2, F_SETFL, FNDELAY);

    internalLog("Both players connected");
}

int getClientData(int socketFd) {
    int val = 0;
    
    int ret = read(socketFd, &val, sizeof(val));
    if (ret == 0) {
        return -1;
    }
    //read(socketFd, &val, 0);
    return val;
}

void sendClientData(int socketFd, char buffer[], int size) {
    send(socketFd, buffer, size, 0);
}

void closeServer(ServerConnection* server) {
    close(server->clientSocket1);
    close(server->clientSocket2);

    shutdown(server->socketFd, SHUT_RDWR);
}


void initClient(ClientConnection* client, char* address, int port) {
    client->socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (client->socketFd < 0) {
        internalLog("Failed to init socket");
        exit(0);
    }

    memset(&client->address, 0, sizeof(client->address));
    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(port);

    client->addressSize = sizeof(client->address);

    int ret = inet_pton(AF_INET, address, &client->address.sin_addr);
    
    if (ret <= 0) {
        internalLog("Failed to convert from text to binary");
    }

    //char service[50];
    //sprintf(service, "%i", port);

    //struct addrinfo* results;
    //ret = getaddrinfo(address, service, (struct addrinfo) &client->address, &results); 
    //ret = getnameinfo((struct sockaddr*) &client->address, client->addressSize, address, strlen(address), NULL, 0, 0);

    //int ret = inet_pton(AF_INET, address, &client->address.sin_addr);

    //if (ret != 0) {
    //    //internalLog("Invalid or unsupported address");
    //    internalLog(gai_strerror(ret));
    //    exit(0);
    //}


    client->clientFd = connect(client->socketFd, (struct sockaddr*) &client->address, client->addressSize);

    if (client->clientFd < 0) {
        internalLog("Failed to connect");
        exit(0);
    }

    fcntl(client->clientFd, F_SETFL, FNDELAY);
}

void getServerData(ClientConnection* client, char buffer[], int size) {
    read(client->socketFd, buffer, size);
}

void sendServerData(ClientConnection* client, int val) {
    send(client->socketFd, &val, sizeof(val), 0);
}

void closeClient(ClientConnection* client) {
    close(client->clientFd);
}

