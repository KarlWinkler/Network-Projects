// for handling requests so I dont have to dig through server connecting shit that looks the same as everything else
// I want to be able to code it seperately, I can combine it later if I need to 
// hopefuly this doesnt create more problems
// :)

#define STRING_WORDS 2000

char* replace_words(char string[STRING_WORDS]){
    for(int i = 0; i < STRING_WORDS; i++){
        if(strcmp(string[i], "\n\r\n\r") == 0){ // \n\r\n\r is EOF
            break; // exit loop
        }

        if(strcmp(string[i], "Happy") == 0){
            strcpy(string[i], "Silly");
        }



    }
}