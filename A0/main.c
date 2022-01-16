#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char message[64], server_reply[2000];
    char * welcome_message = "Welcome to Palindrome Tester\nFind out if any phrase is a paindrome or not\nMax characters (64)\n\n";
    char * input_message = "Enter a word, or 'exit' to quit: ";

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc == -1){
        printf("could not create socket on line\n");
    }
    else{
        printf("created socket %d \n\r", socket_desc);
    }

    server.sin_addr.s_addr = inet_addr("136.159.5.25");
    server.sin_family = AF_INET;
    server.sin_port = htons(44144);

    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        puts("connect error\n");
        return 1;
    }

    puts("connected\n");

    printf("%s", welcome_message);


    while(strcmp(message, "exit") != 0){

        printf("%s", input_message);
        fgets(message, 64, stdin);

        strtok(message, "\n");

        if(strcmp(message, "\n") == 0){
            message[0] = ' ';
        }

        if(strcmp(message, "exit") == 0){
            printf("closed\n");
            break;
        }

        if(send(socket_desc, message, strlen(message), 0) < 0){
            puts("send failed");
        }

        // puts("data send\n");

        if(recv(socket_desc, server_reply , 2000 , 0) < 0){
            puts("recv failed");
        }

        //handle resopnse

        if(server_reply[0] == 'S'){
            printf("%s is NOT a Palindrome\n", message);
        }
        
        if(server_reply[0] == 'Y'){
            printf("YES %s is a Palindrome\n", message);
        }
        // puts("Reply received\n");
        // puts(server_reply);

    }

    close(socket_desc);
    return 0;
};


