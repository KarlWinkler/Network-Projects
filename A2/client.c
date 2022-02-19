#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define MAX_LEN 2000

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in tcp_server, udp_server;
    struct sockaddr * server;
    int s;
    socklen_t len;
    char tcp_message[MAX_LEN], udp_message[MAX_LEN], tcp_reply[MAX_LEN], udp_reply[MAX_LEN];

    // tcp socket
    if((socket_desc = socket(AF_INET, SOCK_STREAM, 0))< 0){
        printf("could not create tcp socket");
        exit(1);
    }

    // udp socket
    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        puts("failed to create udp socket");
        exit(1);
    }

    // tcp setup
    tcp_server.sin_addr.s_addr = inet_addr(SERVER_IP);
    tcp_server.sin_family = AF_INET;
    if(argc > 1){
        tcp_server.sin_port = htons(atoi(argv[1]));

    }
    else{
        tcp_server.sin_port = htons(8080);
    }

    // udp setup
    memset((char *) &udp_server, 0, sizeof(udp_server));
    udp_server.sin_family = AF_INET;
    if(argc > 2){
        udp_server.sin_port = htons(atoi(argv[2]));

    }
    else{
        udp_server.sin_port = htons(9090);
    }   
    server = (struct sockaddr *) &udp_server;
    len = sizeof(udp_server);
    
    // tcp connectsg
    if(connect(socket_desc, (struct sockaddr *)&tcp_server, sizeof(tcp_server)) < 0){
        puts("connect error\n");
        return 1;
    }

    // udp connect
    if(inet_pton(AF_INET, SERVER_IP, &udp_server.sin_addr) == 0){
        puts("udp connect error\n");
        return 1;
    }

    while(134535){

        bzero(udp_message, MAX_LEN);
        bzero(tcp_message, MAX_LEN);
        
        // prompt user for proccess type
        printf("Enter a process (split, merge, quit): ");
        fgets(tcp_message, MAX_LEN, stdin);

        // hack to make empty strings not hang the program
        if(strcmp(tcp_message, "\n") == 0){
            strcpy(tcp_message, " \n");
        }

        // strlen - 1 is to remove newline
        if(send(socket_desc, tcp_message, strlen(tcp_message) - 1, 0) < 0){
            puts("tcp send failed");
        }

        if(sendto(s, " ", 1, 0, server, len) < 0){
            puts("dummy udp failed");
        }

        // if(recvfrom(s, udp_reply, sizeof(udp_reply), 0, server, &len) < 0){
        //     printf("recfrom failed");
        //     exit(1);
        // }

        //recv ACK from server
        if((recv(socket_desc, tcp_reply, sizeof(tcp_reply), 0)) < 0){
            puts("receive failed");
            exit(1);
        }

        // handle the input for each operation

        //quit ======================
        if(strcmp(tcp_reply, "quit") == 0){
            close(socket_desc);
            exit(0);
        }

        //split =====================
        else if(strcmp(tcp_reply, "split") == 0){
            char toSplit[MAX_LEN];
            char encription[MAX_LEN];

            printf("Enter the encription method for split (basic: 0, advanced: 1, best: 2): ");
            fgets(encription, MAX_LEN, stdin);

            if(send(socket_desc, encription, strlen(encription), 0) < 0){
                puts("split tcp send failed");
            }

            printf("Enter the word you want to split: ");
            fgets(toSplit, MAX_LEN, stdin);

            if(send(socket_desc, toSplit, strlen(toSplit), 0) < 0){
                puts("split tcp send failed");
            }

            bzero(udp_reply, MAX_LEN); // to be safe
            if(recvfrom(s, udp_reply, sizeof(udp_reply), 0, server, &len) < 0){
                printf("recfrom failed");
                exit(1);
            }
            
            bzero(tcp_reply, MAX_LEN); // to be safe
            if((recv(socket_desc, tcp_reply, sizeof(tcp_reply), 0)) == -1){
                puts("receive failed");
                exit(1);
            }

            printf("    Vowels: %s\n", udp_reply);
            printf("Non-Vowels: %s\n", tcp_reply);
        }

        // merge ====================
        else if(strcmp(tcp_reply, "merge") == 0){
            char vowels[MAX_LEN];
            char rest[MAX_LEN];

            printf("Enter the VOWELS you want to merge: ");
            fgets(vowels, MAX_LEN, stdin);

            printf("Enter the NON-VOWELS you want to merge: ");
            fgets(rest, MAX_LEN, stdin);

            // send vowels (strlen-1 to remove \n)
            if(sendto(s, vowels, strlen(vowels) - 1, 0, server, len) < 0){
                puts("vowels send failed");
            }

            //send rest (strlen-1 to remove \n)
            if(send(socket_desc, rest, strlen(rest) - 1, 0) < 0){
                puts("tcp send failed");
            }

            bzero(tcp_reply, MAX_LEN);
            if((recv(socket_desc, tcp_reply, sizeof(tcp_reply), 0)) == -1){
                puts("receive failed");
            }

            printf("Merged Word: %s", tcp_reply);
        }

        // default case
        else{
            puts("Please enter a valid process.");
            if((recv(socket_desc, tcp_reply, sizeof(tcp_reply), 0)) == -1){
                puts("receive failed");
            }
            continue;
        }


        //puts(tcp_reply);
        //puts(udp_reply);

   

   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
    }
































}