#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "proxy_func.c"

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 2000
#define REPLY "message received\n"
#define MY_PORT_NUM 8080         /* you can change this if you want! */

/* Main program for proxy */
int main(int argc, char *argv[]){
    char message_in[MAX_MESSAGE_LENGTH], server_reply[MAX_MESSAGE_LENGTH];
    char* proxy_response = REPLY;
    struct sockaddr_in client;
    struct sockaddr_in proxy;
    int accept_socket, client_socket;
    socklen_t name_len;

    // get a socket to accept connections on
    if((accept_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("failed to connect to accepting socket");
        exit(1);
    } 

    // bind to socket
    proxy.sin_family = AF_INET;
    proxy.sin_port = htons(MY_PORT_NUM);
    proxy.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(accept_socket, (struct sockaddr *)&proxy, sizeof(proxy)) < 0){
        puts("bind to port failed");
        exit(1);
    }


    if(listen(accept_socket, 1) != 0){
        puts("listen failed");
        exit(1);
    }

    name_len = sizeof(client);
    if((client_socket = accept(accept_socket, (struct sockaddr *)&client, &name_len)) < 0){
        puts("accept failed");
        exit(1);
    }

    // maintain the connection
    for(;;){

        if(recv(client_socket, message_in, sizeof(message_in), 0) == -1){
            puts("receive failed");
            exit(1);
        }

        if(strcmp(message_in, "exit\r\n") == 0){
            break;
        }

        if(connect(message_in[4], (struct sockaddr *)&proxy, sizeof(proxy)) < 0){
            puts("connect error\n");
            return 1;
        }

        if(send(client_socket, message_in, sizeof(message_in), 0) < 0){
            puts("send failed");
            exit(1);
        }

        replace_words(message_in);

        printf("%s\n", message_in);
        if(send(client_socket, message_in, sizeof(message_in), 0) < 0){
            puts("send failed");
            exit(1);
        }

        bzero(message_in, MAX_MESSAGE_LENGTH);

    }

    close(accept_socket);
    close(client_socket);
    puts("conection ended");
    return 0;
    

}