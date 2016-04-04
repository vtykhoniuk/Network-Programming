/* Forked TCP Echo server.
   It waits for incoming tcp connections on port 777 and echoes back everything
   whatever client sent. For each incoming request new process has been created
   using 'fork' syscall.

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

// exit, free
#include <stdlib.h>

// printf, snprintf
#include <stdio.h>

// bzero
#include <strings.h>

// time_t, cnet
#include <time.h>

#include "unet.h"

#define PORT        7777

int main()
{
    int serverfd;

    serverfd = Socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    Sock_set_wild((SA *) &servaddr);
    Sock_set_port((SA *) &servaddr, PORT);

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

        Close(clientfd);
    }

    Close(serverfd);

    exit(EXIT_SUCCESS);
}
