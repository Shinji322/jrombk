#include "network.h"
#include <curses.h>

void initServer(ServerConnection* main){
    int yes =1;

    if ((main->socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket failure!!\n");
        exit(1);
    }

    if (setsockopt(main->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset(&main->server, 0, sizeof(&main->server));
    memset(&main->dest,0,sizeof(main->dest));



    main->server.sin_family = AF_INET;
    main->server.sin_port = htons(PORT);
    main->server.sin_addr.s_addr = INADDR_ANY; 

    if ((bind(main->socket_fd, (struct sockaddr *)&main->server, sizeof(struct sockaddr )))== -1)    { //sizeof(struct sockaddr) 
        fprintf(stderr, "Binding Failure\n");
        exit(1);
    }

    if ((listen(main->socket_fd, BACKLOG))== -1){
        fprintf(stderr, "Listening Failure\n");
        exit(1);
    }

    
    main->size = sizeof(struct sockaddr_in);
    if ((main->client_fd = accept(main->socket_fd, (struct sockaddr *)&main->dest, &main->size))==-1 ) {
    //if ((main->client_fd = accept(main->socket_fd, NULL, NULL))==-1 ) {
        perror("accept");
        exit(1);
    }

    printf("Server got connection from client %s\n", inet_ntoa(main->dest.sin_addr));

    fcntl(main->client_fd, F_SETFL, fcntl(main->client_fd, F_GETFL, 0) | O_NONBLOCK);    
}

int networkGetch(ServerConnection* main) {
    //main->num = recv(main->client_fd, main->buffer, 4, /*MSG_DONTWAIT*/ 0);
    //mvprintw(0, 0, "Hi:%s", main->buffer);
    if ((main->num = recv(main->client_fd, main->buffer, 4, /*MSG_DONTWAIT*/ 0)) == -1) {
        return 0; 
    }
    //main->buffer[main->num] = '\0';
    //else if (main->num == 0) {
    //    printf("Connection closed\n");
    //    return 0;
    //}

    //printf("Server:Msg Received %s\n", main->buffer);
    //return (int) main->buffer[0]; 
    return ((((int)main->buffer[2]) << 16) | (((int)main->buffer[1]) << 8) | ((int) main->buffer[0]));

    //if ((send(main->client_fd,main->buffer, strlen(main->buffer),0))== -1){
    //    fprintf(stderr, "Failure Sending Message\n");
    //    close(main->client_fd);
    //    return 0;
    //}
    //printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",main->buffer, strlen(main->buffer));
}

void closeServer(ServerConnection* main) {
    close(main->client_fd);
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
            fprintf(stderr, "Failure Sending Message\n");
            close(main->socket_fd);
            exit(1);
    }
    //else {
    //        printf("Client:Message being sent: %s\n",buffer);
    //        num = recv(socket_fd, buffer, sizeof(buffer),0);
    //        if ( num <= 0 )
    //        {
    //                printf("Either Connection Closed or Error\n");
    //                //Break from the While
    //                break;
    //        }

    //        buff[num] = '\0';
    //        printf("Client:Message Received From Server -  %s\n",buffer);
    //   }

}

void closeClient(ClientConnection* main) {
    close(main->socket_fd);
}
