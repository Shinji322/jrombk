#include "network.h"
#include "io.h"
#include "compress.h"

void initServer(ServerConnection* main){
    int yes =1;

    if ((main->socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) { //makes master socket
        fprintf(stderr, "Socket failure!!\n");
        exit(1);
    }

    if (setsockopt(main->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) { //tells socket to allow multiple connections
        perror("setsockopt");
        exit(1);
    }

    memset(&main->server, 0, sizeof(main->server));
    memset(&main->dest,0,sizeof(main->dest));



    main->server.sin_family = AF_INET; // sets ipv4 access
    main->server.sin_port = htons(PORT); //set port number
    main->server.sin_addr.s_addr = INADDR_ANY; //no ip blacklist 

    if ((bind(main->socket_fd, (struct sockaddr *)&main->server, sizeof(struct sockaddr )))== -1)    { //binds master socket 
        fprintf(stderr, "Binding Failure\n");
        exit(1);
    }

    if ((listen(main->socket_fd, BACKLOG))< 0){ //tells socket to listen to port, BACKLOG is # of concurrent connection
        fprintf(stderr, "Listening Failure\n");
        exit(1);
    }

    
    main->size = sizeof(struct sockaddr_in); //sets size to construc size
    if ((main->client1_fd = accept(main->socket_fd, (struct sockaddr *)&main->dest, &main->size))==-1 ) {
        perror("accept");
        exit(1);
    }
    else{
        printf("Player 1 Connected at %s\n", inet_ntoa(main->dest.sin_addr));
    }

    if ((main->client2_fd = accept(main->socket_fd, (struct sockaddr *)&main->dest, &main->size))==-1 ) {
        perror("accept");
        exit(1);
    }
    else{        
        printf("Player 2 Connected at %s\n", inet_ntoa(main->dest.sin_addr));
    }

   
    fcntl(main->client1_fd, F_SETFL, fcntl(main->client1_fd, F_GETFL, 0) | O_NONBLOCK);
    fcntl(main->client2_fd, F_SETFL, fcntl(main->client2_fd, F_GETFL, 0) | O_NONBLOCK);  
}

int networkGetch(ServerConnection* main, bool isPlayerOne) {
    if(isPlayerOne){
        if ((main->num = recv(main->client1_fd, main->buffer, 4, /*MSG_DONTWAIT*/ 0)) == -1) {
            return 0; 
        }
    } else {
        if ((main->num = recv(main->client2_fd, main->buffer, 4, /*MSG_DONTWAIT*/ 0)) == -1) {
            return 0; 
        }
    }
    return ((((int)main->buffer[2]) << 16) | (((int)main->buffer[1]) << 8) | ((int) main->buffer[0]));
}

void sendServerData(ServerConnection* main, bool isPlayerOne){
    char* buff = getScreenArray();
    int size = 0;
    char* compBuff = compress(buff, MAP_HEIGHT * MAP_WIDTH * 2, &size);
    //size_t size = strlen(compBuff);
    if(isPlayerOne){
        //if ((send(main->client1_fd, buff, MAP_HEIGHT * MAP_WIDTH /** 2*/,0))== -1){
        if ((send(main->client1_fd, (const void*) &size, sizeof(size_t),0))== -1){
            fprintf(stderr, "Failure Sending Message\n");
            close(main->client1_fd);
        }
        if ((send(main->client1_fd, compBuff, strlen(compBuff),0))== -1){
            fprintf(stderr, "Failure Sending Message\n");
            close(main->client1_fd);
        }
    }
    else{
        //if ((send(main->client2_fd, buff, MAP_HEIGHT * MAP_WIDTH /** 2*/,0))== -1){
        if ((send(main->client2_fd, (const void*) &size, sizeof(size_t),0))== -1){
            fprintf(stderr, "Failure Sending Message\n");
            close(main->client2_fd);
        }
        if ((send(main->client2_fd, compBuff, strlen(compBuff),0))== -1){
            fprintf(stderr, "Failure Sending Message\n");
            close(main->client2_fd);
        }
    }
}

void closeServer(ServerConnection* main) {
    close(main->client1_fd);
    close(main->client2_fd);
    close(main->socket_fd);
}

void initClient(ClientConnection* main, char* address){   
    if ((main->he = gethostbyname(address))==NULL) {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((main->socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&main->server_info, 0, sizeof(main->server_info));
    main->server_info.sin_family = AF_INET;
    main->server_info.sin_port = htons(PORT);
    main->server_info.sin_addr = *((struct in_addr *)main->he->h_addr);
    if (connect(main->socket_fd, (struct sockaddr *)&main->server_info, sizeof(struct sockaddr))<0) {
        perror("connect");
        exit(1);
    }

    fcntl(main->socket_fd, F_SETFL, fcntl(main->socket_fd, F_GETFL, 0) | O_NONBLOCK);
}

void clientPut(ClientConnection* main, int data) {
    //printf("Client: Enter Data for Server:\n");
    main->buffer[0] = (char) data;
    main->buffer[1] = (char) (data >> 8);
    main->buffer[2] = (char) (data >> 16);
    main->buffer[3] = 0;

    mvprintw(0,0,"%i", data); 
    mvprintw(1,0,"%i", (((((int)main->buffer[2])) << 16) | (((int)main->buffer[1]) << 8) | ((int) main->buffer[0])));


    if ((send(main->socket_fd,main->buffer, /*strlen(main->buffer)*/ 4,0))== -1) {
           //fprintf(stderr, "Failure Sending Message\n");
    }
}

int receiveServerData(ClientConnection* main){
    char* buff = getScreenArray();
    int size;
    if(recv(main->socket_fd, &size, sizeof(size_t),0) <= 0){
        //printf("Either Connection Closed or Error\n");
        return -1;
    }

    char* compBuff = malloc(size * sizeof(char));
    int ret = -1;
    do {
        ret = recv(main->socket_fd, compBuff, size,0);
    } while (ret == -1);

    decompress(compBuff, size, buff, MAP_HEIGHT * MAP_WIDTH * 2);

        
    return 1;
}

void closeClient(ClientConnection* main) {
    close(main->socket_fd);
}
