/* Iterative TCP Echo server.
   This is more universal version of simple TCP echo server
   First difference is that it now takes two command line
   parameters: hostname/IP address and service/port to listen
   on.

   It is protocol independent version, which means the same
   code would work for both: IPv4 and IPv6 addresses

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

// exit, free
#include <stdlib.h>

// printf, snprintf
#include <stdio.h>

// memset
#include <string.h>

// time_t, cnet
#include <time.h>

#include "unet.h"


int main(int argc, char **argv)
{
    int serverfd;

    if (argc != 3)
        err_quit("Usage: %s <Host/IP> <Service/Port>");

    serverfd = tcp_listen(argv[1], argv[2]);

    for (;;) {
        struct sockaddr_storage cliaddr;
        socklen_t cliaddrlen = sizeof(cliaddr);
        int clientfd = Accept(serverfd, (SA *) &cliaddr, &cliaddrlen);

        char *cliaddrstr;
        Sock_ntop((SA *) &cliaddr, &cliaddrstr);
        printf("Connection from [%s]\n", cliaddrstr);
        free(cliaddrstr);

        char msg[MAXLINE];
        time_t t = time(NULL);
        struct tm tm;
        localtime_r(&t, &tm);
        size_t n = strftime(msg, MAXLINE, "%F %T\n", &tm);
        Writen(clientfd, msg, n);
        Close(clientfd);
    }

    Close(serverfd);

    exit(EXIT_SUCCESS);
}
