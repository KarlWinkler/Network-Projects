// HTTP/1.1 proxy
// Author: Karl Winkler
// Date: Jan. 27, 2022

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "proxy_func.c"

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 5068     // maximum length for most char *
#define MY_PORT_NUM 8080            // port proxy runns on

/* Main program for proxy */
int main(int argc, char *argv[]){
    char message_in[MAX_MESSAGE_LENGTH], server_reply[MAX_MESSAGE_LENGTH];
    struct sockaddr_in client;
    struct sockaddr_in proxy;
    struct sockaddr_in server;
    int accept_socket, client_socket, server_socket;
    socklen_t name_len;
    int package_size = 0;
    pid_t pid;

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

    // listen for connections
    if(listen(accept_socket, 1) != 0){
        puts("listen failed");
        exit(1);
    }

    

    // maintain the connection
    while(1){

        name_len = sizeof(client);
        //create connection
        if((client_socket = accept(accept_socket, (struct sockaddr *)&client, &name_len)) < 0){
            puts("accept failed");
            exit(1);
        }

        // create child process
        pid = fork();

        // if this is the child process
        if(pid == 0){
            // we dont need this socket anymore
            close(accept_socket);

            // receive message from client
            if((package_size = recv(client_socket, message_in, sizeof(message_in), 0)) == -1){
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

            // printf("%s\n", message_in);

            //send message to server
            if(send(server_socket, message_in, MAX_MESSAGE_LENGTH, 0) < 0){
                puts("message to server failed");
                exit(1);
            }

            // while receiving message from server, send it on to the client
            while((package_size = recv(server_socket, server_reply, sizeof(server_reply), 0)) > 0){

                // gets the content type from the content type header
                char temp[MAX_MESSAGE_LENGTH];
                strcpy(temp, server_reply);
                char* content_type = get_header_contents(temp, "Content-Type: ");
                //make sure that the packet actually has a content type header
                if(content_type != NULL){
                    // text/html; charset=UTF-8 is the return value for an html file
                    if(strcmp(content_type, "text/html; charset=UTF-8") == 0){
                        do_the_funny(server_reply);
                    }
                    // for jpeg
                    else if(strcmp(content_type, "image/jpeg") == 0){
                        // response is needed so I can strcat() 
                        // everything together for random images to work 
                        // other variables will be assigened based on the 
                        // current time in microseconds
                        char response[100];
                        char img_name[16];
                        char img_size[16];
                        int rand_val;
                        // for storing the result of gettimeofday
                        struct timeval rand;

                        //get time as microseconds since unix epoch
                        gettimeofday(&rand, NULL);

                        // assign values at "random" to get images
                        // yeah, I hardcoded the image size
                        // its already in the issues
                        if((rand_val = rand.tv_usec % 3) == 0){
                            strcpy(img_name, "clown.png");
                            strcpy(img_size, "52930");
                        }
                        else if((rand_val = rand.tv_usec % 3) == 1){
                            strcpy(img_name, "clown2.png");
                            strcpy(img_size, "116686");
                        }
                        else if((rand_val = rand.tv_usec % 3) == 2){
                            strcpy(img_name, "clown3.png");
                            strcpy(img_size, "24761");
                        }
                        
                        // start of response
                        char * start = "HTTP/1.1 200 OK\r\nContent-Length: ";
                        
                        // glue everything together
                        strcat(response, start);
                        strcat(response, img_size);
                        strcat(response, "\r\nConnection: close\r\nContent-Type: image/jpg\r\n\r\n");
                        // FORMAT:
                        // HTTP/1.1 200 OK
                        // Content-Length: 421
                        // Connection: close
                        // Content-Type: text/html; charset=UTF-8
                        
                        // return header response to client (I send the rest later)
                        if(send(client_socket, response, strlen(response), 0) < 0){
                            puts("message to server failed");
                            exit(1);
                        }

                        //create buffer, FILE and byte variable
                        FILE * fp;
                        char buffer[MAX_LENGTH];
                        int bytes = 0;
                        
                        //open file pointer
                        fp = fopen(img_name, "rb");

                        // read bits from image file and send them to the client
                        while((bytes = fread(buffer, 1, MAX_LENGTH, fp)) > 0){
                            //puts("buffer");
                            if(send(client_socket, buffer, bytes, 0) < 0){
                                puts("message to server failed");
                                exit(1);
                            }
                        }
                        // don't try to recieve or send anymore (we are done)
                        break;
                    }
                }

                // send to client if it is not replacing a jpeg
                if(send(client_socket, server_reply, package_size, 0) < 0){
                    puts("return to client failed");
                    exit(1);
                }
                //clear string so we dont corrupt them
                bzero(server_reply, sizeof(server_reply));
                // sleep(0.01);
            }
            
            // we dont need this socket anymore
            close(client_socket);
            // kill the child process
            exit(0);
            
        }

    }

    // shouldnt ever really reach here I think
    puts("conection ended"); 
    return 0;
    

}
