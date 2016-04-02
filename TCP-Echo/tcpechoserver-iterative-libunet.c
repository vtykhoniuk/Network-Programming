/* Iterative TCP Echo server.
   It waits for incoming tcp connections on port 17 and processes them iteratively
   one by one. No additional libs are used: just plain UNIX network interface and
   generic libc.

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

// exit
#include <stdlib.h>

// printf, snprintf
#include <stdio.h>

// close
#include <unistd.h>

// inet_ntop
#include <arpa/inet.h>

// bzero
#include <strings.h>

#include "unet.h"

#define BUFFER_MAX  1024
#define PORT        17

int main()
{
    int serverfd;

    serverfd = Socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    Bind(serverfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(serverfd, 5);

    for (;;) {
        struct sockaddr_in cliaddr;
        char *cliaddrstr;

        socklen_t cliaddrlen = sizeof(cliaddr);
        int clientfd = Accept(serverfd, (SA *) &cliaddr, &cliaddrlen);

        cliaddrstr = Sock_ntop((SA *) &cliaddr);
        printf("Connection from [%s]\n", cliaddrstr);
        free(cliaddrstr);

        char msg[BUFFER_MAX];
        int msglen = snprintf(msg, BUFFER_MAX, "Hello, World!\r\n");
        writen(clientfd, msg, msglen);
        close(clientfd);
    }

    close(serverfd);

    exit(EXIT_SUCCESS);
}
