// fileRedaer.c 
// reads file to array
//
// Author: Karl Winkler
// Date: 6-03-22

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 1024 // maximum line length, quite a bit longer than expected
#define MAX_LINES 1048576 // maximum lines in a file 2^20 should be enough

char** file_reader(char* file_path, long* len, int *err){
    
    long i;
    *err = 0;

    char **lines = (char**)malloc(MAX_LINES * sizeof(char *));
    for(i = 0; i < MAX_LINES; i++){
        lines[i] = (char*)malloc(MAX_LEN); // sizeof(char) is 1 
    }

    FILE *fp;
    
    if((fp = fopen(file_path, "r")) == NULL){
        printf("%s cannot be found", file_path);
        *err = 1;
    }

    i = 0;
    while(fgets(lines[i], MAX_LEN, fp) != NULL){
        i++;
    }

    *len = i;

    return lines;
}

void destroy_array(char** arr){
    for (int i = 0; i < MAX_LINES; i++)
        free(arr[i]);
    free(arr);
}