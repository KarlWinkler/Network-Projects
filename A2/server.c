#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "helper.c"

#define MAX_MESSAGE_LENGTH 2000     // maximum length for most char *
#define MY_PORT_NUM 8080            // port server runs on

/* Main program for server */
int main(int argc, char *argv[]){
    char udp_buffer[MAX_MESSAGE_LENGTH], tcp_buffer[MAX_MESSAGE_LENGTH];
    struct sockaddr_in tcp_client, udp_client, tcp_server, udp_server;
    struct sockaddr * server, * client;
    int accept_socket, client_socket, udp_socket;
    socklen_t name_len, len;
    int package_size = 0;
    pid_t pid;

    // =====
    // create TCP server
    // =====

    // get a socket to accept connections on
    if((accept_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("failed to connect to accepting socket");
        exit(1);
    } 

    // bind to socket
    tcp_server.sin_family = AF_INET;

    // use args for port number
    if(argc > 1){
        tcp_server.sin_port = htons(atoi(argv[1]));
    }
    else{
        tcp_server.sin_port = htons(MY_PORT_NUM);
    }
    
    tcp_server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(accept_socket, (struct sockaddr *)&tcp_server, sizeof(tcp_server)) < 0){
        puts("bind to port failed");
        exit(1);
    }

    // =====
    // create UDP server
    // =====

    if((udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        puts("udp fucky wucky");
        exit(1);
    }

    memset(&udp_server, 0, sizeof(udp_server));
    udp_server.sin_family = AF_INET;
    if(argc > 2){
        udp_server.sin_port = htons(atoi(argv[2]));
        // puts(argv[2]);
    }
    else{
        udp_server.sin_port = htons(9090);
    }   
    udp_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &udp_server;
    client = (struct sockaddr *) &udp_client;
    
    if(bind(udp_socket, server, sizeof(udp_server)) < 0){
        puts("udp connect error\n");
        return 1;
    }

    len = sizeof(udp_client);

    // listen for tcp connections
    if(listen(accept_socket, 1) != 0){
        puts("listen failed");
        exit(1);
    }

    while(1){

        name_len = sizeof(tcp_client);
        //create connection to client
        if((client_socket = accept(accept_socket, (struct sockaddr *)&tcp_client, &name_len)) < 0){
            puts("accept failed");
            exit(1);
        }

        // create child process
        pid = fork();

        // if this is the child process
        if(pid == 0){
            // we dont need this socket anymore
            close(accept_socket);

            while (1){

                bzero(udp_buffer, MAX_MESSAGE_LENGTH);
                bzero(tcp_buffer, MAX_MESSAGE_LENGTH);

                puts("receiving...");

                if((package_size = recv(client_socket, tcp_buffer, sizeof(tcp_buffer), 0)) < 0){
                    puts("receive failed");
                }

                if(recvfrom(udp_socket, udp_buffer, sizeof(udp_buffer), 0, client, &len) < 0){
                    printf("dummy udp recv failed");
                    exit(1);
                }

                // if(sendto(udp_socket, udp_buffer, sizeof(udp_buffer), 0, client, len) < 0){
                //     puts("udp vowels send failed");
                //     printf("%d", errno);
                // }
                
                // send ACK
                if(send(client_socket, tcp_buffer, sizeof(tcp_buffer), 0) < 0){
                    puts("send fail");
                }

                // if ======= quit ===========
                if(strcmp(tcp_buffer, "quit") == 0){
                    if(send(client_socket, tcp_buffer, sizeof(tcp_buffer), 0) < 0){
                        puts("send fail");
                    }
                    close(client_socket);
                    exit(0);
                }

                // if ======= split ==========
                else if(strcmp(tcp_buffer, "split") == 0){
                    char vowels[MAX_MESSAGE_LENGTH];
                    char rest[MAX_MESSAGE_LENGTH];
                    char encription[MAX_MESSAGE_LENGTH];

                    //wait for encription method
                    if((package_size = recv(client_socket, encription, sizeof(encription), 0)) == -1){
                        puts("receive failed");
                    }

                    //wait for string to parse
                    bzero(tcp_buffer, MAX_MESSAGE_LENGTH); // safe
                    if((package_size = recv(client_socket, tcp_buffer, sizeof(tcp_buffer), 0)) == -1){
                        puts("receive failed");
                    }

                    // printf(tcp_buffer);

                    split(tcp_buffer, vowels, rest, atoi(encription));

                    puts(vowels);
                    puts(rest);

                    if(sendto(udp_socket, vowels, strlen(vowels), 0, client, len) < 0){
                        puts("udp vowels send failed");
                        printf("%d", errno);
                    }
                    
                    if(send(client_socket, rest, strlen(rest), 0) < 0){
                        puts("send fail");
                    }
                    
                }
                // if ======= merge ==========
                else if(strcmp(tcp_buffer, "merge") == 0){
                    char vowels[MAX_MESSAGE_LENGTH];
                    char rest[MAX_MESSAGE_LENGTH];

                    if(recvfrom(udp_socket, vowels, sizeof(vowels), 0, client, &len) < 0){
                        printf("dummy udp recv failed");
                        exit(1);
                    }



                    bzero(tcp_buffer, MAX_MESSAGE_LENGTH); // safe
                    if((package_size = recv(client_socket, rest, sizeof(rest), 0)) < 0){
                        puts("receive failed");
                    }
                    
                    bzero(tcp_buffer, MAX_MESSAGE_LENGTH);
                    merge(tcp_buffer, vowels, rest);

                    if(send(client_socket, tcp_buffer, sizeof(tcp_buffer), 0) < 0){
                        puts("send fail");
                    }
                }
                else{
                    exit(0);
                }
            }
        }
    }
}