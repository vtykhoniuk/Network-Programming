/* Forked UDP Echo server.
   Iterative UDP echo server. It echoes back every UDP packet arrived to the host

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

// exit, free
#include <stdlib.h>

// memset
#include <string.h>

#include "unet.h"

#define PORT        7777

int main()
{
    int servfd = Socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    Sock_set_wild((SA *) &servaddr);
    Sock_set_port((SA *) &servaddr, PORT);

    Bind(servfd, (SA *) &servaddr, sizeof(servaddr));

    char buf[MAXLINE];
    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t cliaddrlen = sizeof(&cliaddr);

        size_t n = Recvfrom(servfd, buf, MAXLINE, 0, (SA *) &cliaddr, &cliaddrlen);
        char *cliaddr_str;
        Sock_ntop((SA *) &cliaddr, &cliaddr_str);

        fprintf(stderr, "UDP packet of size %ld bytes received from [%s]\n", n, cliaddr_str);
        free(cliaddr_str);

        Sendto(servfd, buf, n, 0, (SA *) &cliaddr, cliaddrlen);
    }

    Close(servfd);
}
