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

int main(void){
    initNcurses();

    struct sockaddr_in serv; //This is our main socket variable.
    int fd; //This is the socket file descriptor that will be used to identify the socket
    int conn; //This is the connection file descriptor that will be used to distinguish client connections.
    char messageOut[100] = ""; //This array will store the messages that are sent by the server
    char messageIn[100] = "";
    int num;

    //char recvBuff[1024]; //this is all set to 0
    int n = 0;
    //memset(recvBuff, '0' ,sizeof(recvBuff));

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8096);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr); //This binds the client to localhost

    connect(fd, (struct sockaddr *)&serv, sizeof(serv)); //This connects the client to the server.

    while(1){
        do{
            messageOut[0] = getch();
        } while(messageOut[0] == ERR);

        send(fd, messageOut, strlen(messageOut), 0);

        //num = recv(fd, messageIn, sizeof(messageIn),0);
        //messageIn[num] = '\0';
        //printf("Client:Message Received From Server -  %s\n",messageIn);

    }
}

//(n = read(fd, recvBuff, sizeof(recvBuff)-1)) > 0) {