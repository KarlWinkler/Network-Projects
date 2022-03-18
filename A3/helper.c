#include <stdio.h>
#include <stdlib.h>

#include "packet.h"

char *strsplit(char* str, char delim){
    int i = 0;
    char *ret = (char *)malloc(32);
    while(str[i] != '\0' && str[i] != delim){
        ret[i] = str[i];
        i++;
    }

    return ret;
}

char *strend(char* str, char delim){
    int i = 0;
    int j = 0;
    char *ret = (char *)malloc(32);
    while(str[i] != '\0' && str[i] != delim){
        i++;
    }
    while(str[i] != '\0'){
        ret[j]  = str[i];
        i++;
        j++;
    }

    return ret;
}

struct packet* merge_sort(struct packet* a, long a_len, struct packet* b, long b_len){
    struct packet *res = malloc((a_len+b_len) * sizeof(struct packet));
    long inx_a = 0, inx_b = 0, i = 0;
    for(i = 0; i < a_len && i < b_len; i++){
        if(a[inx_a].arrival_time < b[inx_b].arrival_time){
            res[i] = a[inx_a];
            inx_a++;
        }
        else{
            res[i] = b[inx_b];
            inx_b++;
        }

    }

    while(inx_a < a_len){
        res[i] = a[inx_a];
        inx_a++;
        i++;
    }
    while(inx_b < b_len){
        res[i] = b[inx_b];
        inx_b++;
        i++;
    }

    return res;
}   

struct packet *toPackets(char** trace, long len, long *sum_in){
    struct packet *packets = malloc(len * sizeof(struct packet));
    for(long i = 0; i < len; i++){

        char *time = strsplit(trace[i], ' ');

        char *size = strend(trace[i], ' '); 

        packets[i].arrival_time = atof(time);
        packets[i].size = (atoi(size));
        *sum_in += packets[i].size;

    }
    return packets;
}