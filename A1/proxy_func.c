#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for handling requests so I dont have to dig through server connecting shit that looks the same as everything else
// I want to be able to code it seperately, I can combine it later if I need to 
// hopefuly this doesnt create more problems (it did and didn't :/)
// better readability as well

#define MAX_LENGTH 5068 // same as max message length in proxy.c

// *you probably want to give this a deep copied string*
// message should be the HTTP message
// header should be in the form "Header: "
char* get_header_contents(char* message, char* header){

    char* res; // result  

    // go to the start of the header
    if((res = strstr(message, header)) == NULL){
        return NULL;
    }

    // break the string at the end of the header
    res = strtok(res, "\n\r");

    // moves to the start of contents 
    res = res+strlen(header);    

    return res;
}

// replaces words apropriately
char* do_the_funny(char* message){
    
    char *res = (strstr(message, "\r\n\r\n")); // move to contents
    while((res = strstr(res, "appy")) != NULL){ // match [H/h]appy 
        // replace first letter depending on capitalization
        if(res[-1] == 'h'){
            res[-1] = 's';
        }
        else if(res[-1] == 'H'){
            res[-1] = 'S';
        }
        else{
            break;
        }
        //replace the rest of the word
        res[0] = 'i';
        res[1] = 'l';
        res[2] = 'l';
        res[3] = 'y';
        res += 5;
    }
    return message;
}