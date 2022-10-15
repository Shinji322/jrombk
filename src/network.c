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
#include <ncurses.h>

#define PORT 3490
#define MAXSIZE 1024

void initColor(){
    // Alow the use of color
    use_default_colors();
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
}

void initNcurses(){
    // Init curses
    initscr(); 
    // Disable buffering and get one character-at-a-time input
    cbreak(); 
    // Get Special keys
    keypad(stdscr, TRUE);
    // Suppress echoing of typed keys
    noecho();
    // Hide cursor
    curs_set(0);
    initColor();

    nodelay(stdscr, true);
}

void initServer(){
    struct sockaddr_in server;
    struct sockaddr_in dest;
    int status,socket_fd, client_fd,num;
    socklen_t size;

    char buffer[10241];
    char *buff;
    int yes =1;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket failure!!\n");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    memset(&dest,0,sizeof(dest));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; 

    if ((bind(socket_fd, (struct sockaddr *)&server, sizeof(struct sockaddr )))== -1)    { //sizeof(struct sockaddr) 
        fprintf(stderr, "Binding Failure\n");
        exit(1);
    }

    if ((listen(socket_fd, BACKLOG))== -1){
        fprintf(stderr, "Listening Failure\n");
        exit(1);
    }

    while(1) {
        size = sizeof(struct sockaddr_in);
        if ((client_fd = accept(socket_fd, (struct sockaddr *)&dest, &size))==-1 ) {
            perror("accept");
            exit(1);
        }
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
        while(1){ 
            if ((num = recv(client_fd, buffer, 1024,0))== -1) {
                perror("recv");
                exit(1);
            }
            else if (num == 0) {
                printf("Connection closed\n");
                break;
            }

            buffer[num] = '\0';
            printf("Server:Msg Received %s\n", buffer);

            if ((send(client_fd,buffer, strlen(buffer),0))== -1){
                fprintf(stderr, "Failure Sending Message\n");
                close(client_fd);
                break;
            }
            printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",buffer, strlen(buffer));
        }
        close(client_fd);
    }
    close(socket_fd);
    return 0;
}

void initClient(char* address){   
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;
    char buffer[1024];
    char buff[1024];

    if (argc != 2) {
        fprintf(stderr, "Usage: client hostname\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1]))==NULL) {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr = *((struct in_addr *)he->h_addr);
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) {
        perror("connect");
        exit(1);
    }

    while(1) {
        printf("Client: Enter Data for Server:\n");
        do{
            buffer[0] = getch();
        } while(buffer[0] == ERR);

        if ((send(socket_fd,buffer, strlen(buffer),0))== -1) {
                fprintf(stderr, "Failure Sending Message\n");
                close(socket_fd);
                exit(1);
        }
        else {
                printf("Client:Message being sent: %s\n",buffer);
                num = recv(socket_fd, buffer, sizeof(buffer),0);
                if ( num <= 0 )
                {
                        printf("Either Connection Closed or Error\n");
                        //Break from the While
                        break;
                }

                buff[num] = '\0';
                printf("Client:Message Received From Server -  %s\n",buffer);
           }
    }
    close(socket_fd);
}
