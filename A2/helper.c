#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

// char is int
int cii(char c){
    if(c >= '0' && c <= '9'){
        return 1;
    }
    else{
        return 0;
    }
}

// char is vowel
int civ(char c){
    if( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
        c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ){
            return 1;
    }
    else{
        return 0;
    }
}

// expands integers in the string that represent repeated characters
void expand_str(char* str){
    char res[9*strlen(str)];
    int k = 0; // res counter

    for(int i = 0; i < strlen(str); i++){
        if(cii(str[i]) != 0){
            for(int j = 0; j < (str[i] - '0'); j++){
                res[k] = str[i-1];
                k++;
            }
        }
        else{
            res[k] = str[i];
            k++;
        }
    }

    strcpy(str, res);
}

// compress the repeated characters to ints
void compress_str(char* str){
    char res[strlen(str)];
    char last = str[0];
    int i = 1;
    int c;
    int k = 0; // res counter

    while(i <= strlen(str)){
        c = 0;
        while(str[i] == last && i < strlen(str)){
            c++;
            i++;
        }
        if(c > 0){
            res[k] = last;
            res[k+1] = c + '0';
            k+=2;
        }   
        else{
            res[k] = last;
            k++;
        }

        last = str[i];
        i++;
    }
    res[k] = '\0';

   //  printf("%c, %ld", res[9], k);
    strcpy(str, res);
}

int basic_split(char * str, char* vowels, char* rest){
    char v[strlen(str)]; //vowels
    char r[strlen(str)]; //rest
    int i = 0;

    for(i = 0; i < strlen(str); i++){
        if( civ(str[i]) ){
                v[i] = str[i];
                r[i] = ' ';
        }
        else{
            v[i] = ' ';
            r[i] = str[i];
        }
    }

    v[i] = '\0';
    r[i] = '\0';
    
    strcpy(vowels, v);
    strcpy(rest, r);
    
    return 0;
}

int advanced_split(char * str, char* vowels, char* rest){
    char v[strlen(str)]; //vowels
    char r[strlen(str)]; //rest
    int vc = 0, rc = 0;
    int i = 0;
    int c; // count for non vowels

    while(i < strlen(str)){

        if( civ(str[i]) ){
            v[vc] = str[i];
            vc++;
            i++;
        }

        // see how many non vowels are in a row
        c = 0;
        while( !civ(str[i]) && i < strlen(str)+1 && (str[i] != '\0')){ 
            r[rc] = str[i];
            // increment str, rest-count, non-vowel-counter
            rc++;
            c++;
            i++;
        }
        // because I have to I guess
        if(i >= strlen(str)){
            c--;
        }

        v[vc] = c + '0';
        vc++;
    }

    v[vc] = '\0';
    r[rc] = '\0';
    
    strcpy(vowels, v);
    strcpy(rest, r);

    return 0;
}

int best_split(char * str, char* vowels, char* rest){
    char v[strlen(str)]; //vowels
    char r[strlen(str)]; //rest
    int vc = 0, rc = 0;
    int i = 0;
    int c; // count for non vowels

    while(i < strlen(str)){

        if( civ(str[i]) ){
            v[vc] = str[i];
            vc++;
            i++;
        }

        // see how many non vowels are in a row
        c = 0;
        while( !civ(str[i]) && i < strlen(str)+1 && (str[i] != '\0')){ 
            r[rc] = str[i];
            // increment str, rest-count, non-vowel-counter
            rc++;
            c++;
            i++;
        }
        // because I have to I guess
        if(i >= strlen(str)){
            c--;
        }

        if(c > 0){
            v[vc] = c + '0';
            vc++;
        }

    }

    v[vc] = '\0';
    r[rc] = '\0';

    compress_str(r);
    
    strcpy(vowels, v);
    strcpy(rest, r);

    return 0;
}

int split(char * str, char* vowels, char* rest, int flag){
    if(flag == 0){
        basic_split(str, vowels, rest);
    }
    else if(flag == 1){
        advanced_split(str, vowels, rest);
    }
    else if(flag == 2){
        best_split(str, vowels, rest);
    }
    
    return 0;

}

char* basic_merge(char* merged, char* vowels, char* rest){

    for(int i = 0; i < strlen(vowels); i++){
        if(vowels[i] != ' '){
            merged[i] = vowels[i];
        }
        else{
            merged[i] = rest[i]; 
        }
    }

    // printf("%ld", strlen(vowels) + strlen(rest));

    return merged;

}

char* advanced_merge(char* merged, char* vowels, char* rest){
    int i = 0; // vowels counter
    int j = 0; // merged counter
    int l = 0; // rest counter
    for(i = 0; i < strlen(vowels); i++){
        
        // printf("%c is %d a int and is at index %d, j is %d\n", vowels[i], cii(vowels[i]), i, j);

        // try cii (char is int) if vowels[i] is an int
        if(cii(vowels[i]) == 0){

            merged[j] = vowels[i];
            j++;
        }
        else{
            for(int k = 0; k < (vowels[i] - '0'); k++){
                merged[j] = rest[l];
                j++;
                l++;
            }
        }
    }
    return merged;
}

char* best_merge(char* merged, char* vowels, char* rest){
    int i = 0; // vowels counter
    int j = 0; // merged counter
    int l = 0; // rest counter
    expand_str(rest); // only difference

    for(i = 0; i < strlen(vowels); i++){
        
        printf("%c is %d a int and is at index %d, j is %d\n", vowels[i], cii(vowels[i]), i, j);

        // try cii (char is int) if vowels[i] is an int
        if(cii(vowels[i]) == 0){

            merged[j] = vowels[i];
            j++;
        }
        else{
            for(int k = 0; k < (vowels[i] - '0'); k++){
                merged[j] = rest[l];
                j++;
                l++;
            }
        }
    }
    return merged;
}

char* merge(char* merged, char* vowels, char* rest){
    int merge_type = 0;
    
    for(int i = 0; i < strlen(vowels); i++){
        if(cii(vowels[i]) != 0){
            merge_type = 1;
            break;
        }
    }

    for(int i = 0; i < strlen(rest); i++){
        if(cii(vowels[i]) != 0){
            merge_type = 2;
            break;
        }
    }

    if(merge_type == 0){
        return basic_merge(merged, vowels, rest);
    }

    if(merge_type == 1){
        return advanced_merge(merged, vowels, rest);
    }

    if(merge_type == 2){
        return best_merge(merged, vowels, rest);
    }

    return NULL;

}

