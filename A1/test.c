#include<stdio.h> //printf
#include<string.h> //strcpy
#include<sys/socket.h>
#include<netdb.h>	//hostent
#include<arpa/inet.h>

int main(int argc , char *argv[])
{
	char* string;

    strcpy(string, "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test4.html HTTP/1.1\n\rHost: pages.cpsc.ucalgary.ca\n\rUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:96.0) Gecko/20100101 Firefox/96.0\n\rAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n\rAccept-Language: en-CA,en-US;q=0.7,en;q=0.3\n\rAccept-Encoding: gzip, deflate\n\rConnection: keep-alive\n\rUpgrade-Insecure-Requests: 1");

    char* host = strstr(string, "Host");

    host = strtok(host, "\n\r");

    host = host+6;    

    printf("%s\n", host);
}