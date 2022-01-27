#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for handling requests so I dont have to dig through server connecting shit that looks the same as everything else
// I want to be able to code it seperately, I can combine it later if I need to 
// hopefuly this doesnt create more problems
// :)

#define MAX_LENGTH 5068 // same as max message length
#define CLOWN_FILE_LENGTH 52930

char* get_header_contents(char* message, char* header){
    // message should be the HTTP message
    // header should be in the form "Header: "
    char* res; // gets the header 


    if((res = strstr(message, header)) == NULL){
        return NULL;
    }

    res = strtok(res, "\n\r"); // finds end of header

    res = res+strlen(header);  // moves pointer to the start of contents    

    return res;
}

char* do_the_funny(char* message){
    
    char *res = (strstr(message, "\r\n\r\n")); // move to contents
    while((res = strstr(res, "appy")) != NULL){ //change each char
        if(res[-1] == 'h'){
            res[-1] = 's';
        }
        else if(res[-1] == 'H'){
            res[-1] = 'S';
        }
        else{
            break;
        }
        res[0] = 'i';
        res[1] = 'l';
        res[2] = 'l';
        res[3] = 'y';
        res += 5;
    }
    return message;
}

void send_clown(int socket){

    FILE * fp;
    char buffer[MAX_LENGTH];
    int bytes = 0;

    fp = fopen("clown.png", "rb");

    char * response = "HTTP/1.1 200 OK\r\nContent-Length: 52930\r\nConnection: close\r\nContent-Type: image/jpg\r\n\r\n";
    // HTTP/1.1 200 OK
    // Content-Length: 421
    // Connection: close
    // Content-Type: text/html; charset=UTF-8

    bytes = fread(buffer, 1, MAX_LENGTH - strlen(response), fp);

    strcat(response, buffer);

    printf("%s", response);

    if(send(socket, response, MAX_LENGTH, 0) < 0){
        puts("message to server failed");
        exit(1);
    }

    while((bytes = fread(buffer, 1, MAX_LENGTH, fp)) > 0){
        puts("read");
        if(send(socket, buffer, bytes, 0) < 0){
            puts("message to server failed");
            exit(1);
        }
    }
}