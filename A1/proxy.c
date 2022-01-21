#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "proxy_func.c"

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 5068
#define REPLY "message received\n"
#define MY_PORT_NUM 8080         /* you can change this if you want! */

/* Main program for proxy */
int main(int argc, char *argv[]){
    char message_in[MAX_MESSAGE_LENGTH], server_reply[MAX_MESSAGE_LENGTH];
    char* proxy_response = REPLY;
    struct sockaddr_in client;
    struct sockaddr_in proxy;
    struct sockaddr_in server;
    int accept_socket, client_socket, server_socket;
    socklen_t name_len;

    // =====
    // proxy
    // =====

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

    

    // maintain the connection
    for(;;){

        name_len = sizeof(client);
        if((client_socket = accept(accept_socket, (struct sockaddr *)&client, &name_len)) < 0){
            puts("accept failed");
            exit(1);
        }

        // receive message from client
        if(recv(client_socket, message_in, sizeof(message_in), 0) == -1){
            puts("receive failed");
            exit(1);
        }

        // ======
        // server
        // ======

        // get host from message
        char temp[MAX_MESSAGE_LENGTH];
        strcpy(temp, message_in);
        char* host = get_header_contents(temp, "Host: ");
        
        // set up server settings to host

        struct hostent *address;
        address = gethostbyname(host);

        //printf("%s", host);

        bcopy((char *)address->h_addr, (char *)&server.sin_addr.s_addr, address->h_length);
        // server.sin_addr.s_addr = inet_addr();
        server.sin_family = AF_INET;
        server.sin_port = htons(80);

        //connect to socket
        if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            puts("failed to connect to accepting socket");
            exit(1);
        } 

        // create connection to host
        if(connect(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0){
            puts("connect to server error\n");
            return 1;
        }

        printf("%s\n", message_in);

        //send message to server
        if(send(server_socket, message_in, sizeof(message_in), 0) < 0){
            puts("message to server failed");
            exit(1);
        }


        // receive message from server
        if(recv(server_socket, server_reply, sizeof(server_reply), 0) == -1){
            puts("receive from server failed");
            exit(1);
        }

        // client
        if(send(client_socket, server_reply, sizeof(server_reply), 0) < 0){
                puts("return to client failed");
                exit(1);
        }

        long int file_length = MAX_MESSAGE_LENGTH;
        char* temp_header_content;

        if((temp_header_content = get_header_contents(server_reply, "Content-Length: ")) != NULL){
            int temp_file_length = atoi(temp_header_content);
            if(temp_file_length > MAX_MESSAGE_LENGTH){
                file_length = temp_file_length;
            }
        }

        long int bytes_sent = sizeof(server_reply);

        if(file_length > bytes_sent){
            while(file_length > bytes_sent){
                int package_size = 0;
                
                int current = 0;
                // while(current < MAX_MESSAGE_LENGTH){
                //     printf("%c", server_reply[current]);
                //     current++;
                // }
                bzero(server_reply, sizeof(server_reply)); // clear string
                if((package_size = recv(server_socket, server_reply, MAX_MESSAGE_LENGTH, 0)) == -1){
                    puts("receive from server failed");
                    exit(1);
                }

                // printf("\n========%ld=======\n========%ld/%ld=========", package_size, bytes_sent, file_length);
                // ======
                // client
                // ======
                if(send(client_socket, server_reply, package_size, 0) < 0){
                    puts("return to client failed");
                    exit(1);
                }
                bytes_sent += package_size; // increment bytes sent
            }
        }
        else{
            // ======
            // client
            // ======
            // if(send(client_socket, server_reply, sizeof(server_reply), 0) < 0){
            //     puts("return to client failed");
            //     exit(1);
            // }
        }


        // replace_words(message_in);
        
        printf("%s\n", server_reply);
        // bzero(message_in, MAX_MESSAGE_LENGTH);

        puts("end");

    }

    close(accept_socket);
    close(client_socket);
    puts("conection ended");
    return 0;
    

}