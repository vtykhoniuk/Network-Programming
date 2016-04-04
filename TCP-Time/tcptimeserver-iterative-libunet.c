/* Iterative TCP Echo server.
   It waits for incoming tcp connections on port 17 and returns nothing more but
   current time. It processes incoming connections iteratively one by one, using
   single process. libcerror and, what's more important, libunet are used for
   error processing and interaction with UNIX kernel.

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

#define MAXLINE  1024
#define PORT        17

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

        char msg[MAXLINE];
        time_t t = time(NULL);
        if (ctime_r(&t, msg) == NULL) {
            err_quit("cnet_r error");
            strcpy(msg, "Internal error\n");
        }

        Writen(clientfd, msg, strlen(msg));
        Close(clientfd);
    }

    Close(serverfd);

    exit(EXIT_SUCCESS);
}
