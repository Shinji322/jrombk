#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ncurses.h>

int main(void){
    struct sockaddr_in serv; //This is our main socket variable.
    int fd; //This is the socket file descriptor that will be used to identify the socket
    int connection; //This is the connection file descriptor that will be used to distinguish client connections.
    char buffer[10241];
    int n;
    
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8096); //Define the port at which the server will listen for connections.
    serv.sin_addr.s_addr = INADDR_ANY;
    
    fd = socket(AF_INET, SOCK_STREAM, 0); //This will create a new socket and also return the identifier of the socket into fd.
    
    bind(fd, (struct sockaddr *)&serv, sizeof(serv)); //assigns the address specified by serv to the socket
    
    listen(fd,2); //Listen for client connections. Maximum 2 connections will be permitted be cause our game is 1v1
    
    while(connection = accept(fd, (struct sockaddr *)NULL, NULL)) {        
        int pid;
        if((pid = fork()) == 0) {
            //while (recv(connection, message, 100, 0) > 0) {
                //message[1] = 0;
                //printf("Message Received: %s\n", message);
                //send(fd,message, strlen(message), 0);
            //}
            
            while(1){
                recv(connection, buffer, 100, 0);
                buffer[1] = 0;
                printf("Message Received: %s\n", buffer);
                /*
                if ((send(fd,buffer, strlen(buffer),0))== -1) 
                {
                     fprintf(stderr, "Failure Sending Message\n");
                     close(fd);
                     break;
                }
                */
                //send(fd,message, strlen(message), 0);
            }

            //exit(0);
        }
    }
}