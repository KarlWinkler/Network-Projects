# HTTP/1.1 Clown Proxy by Karl Winkler 
##### January 27, 2022

## Description
This proxy will replace the following words in any HTML file requested by 
an HTTP page using this proxy:
happy -> silly 
Happy -> Silly

The proxy will also change jpeg images to one of the three png images
included in this directory.

## How to use 

set up your system to use the proxy on port 8080
from the IP you are running it from.

eg. 127.0.0.1 8080

RUN cc -o proxy proxy.c
RUN ./proxy

Don't forget to disable the proxy when you are done using it.


### debug notes:
- Make sure your browser doesn't redirect HTTP to HTTPS
- Make sure the cache is cleared or disabled entirely 
