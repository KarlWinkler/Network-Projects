# Vowelizer
#### Febuary 18, 2022

## Description
The client will let you choose a process you want to perform

split - select a mode

The different modes will produce the different split types. 
It will then prompt for the string that you want to split, returning it back to 
you with seperaetd vowels and the rest of the letters.

merge - will accept any of three formats for letters
matching with the strings produced by the split command

quit - exits the program

## How to use 

edit the ip in client.c on line 13 to match the ip the server is going to run on

RUN cc -o client client.c
RUN cc -o server server.c

RUN ./client 
RUN ./server

** note about running client and server
you can pass the TCP port and UDP port 
as command line arguments if you 
don't want to/ can't run them on the
default ports, 8080 and 9090.

eg. 
./client 1234 1232
./server 1234 1232


### testing and other
Testing was done on my local linux laptop running ubuntu LTS. Everything works as 
outlined in the requirements document although I did not implement package loss handeling.
