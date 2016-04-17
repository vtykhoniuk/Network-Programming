/* TCP Echo Client
   This is almost the same clien as tcpechoclient02
   It's now possible to pass hostname instead of IP address
   and specify either port number or service name in a command line

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

// exit, free
#include <stdlib.h>

// memset
#include <string.h>

#include "unet.h"


int main(int argc, char *argv[])
{
    if (argc != 3)
        err_quit("Usage: %s <Host/IP> <Service/Port>", argv[0]);

    char buf[MAXLINE];
    int sockfd = tcp_connect(argv[1], argv[2]);

    int stdineof = 0;
    for (;;) {
        size_t n;
        fd_set readset;
        FD_ZERO(&readset);
        if (stdineof == 0)
            FD_SET(STDIN_FILENO, &readset);
        FD_SET(sockfd, &readset);

        Select(sockfd+1, &readset, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &readset)) {
            if ((n = Read(STDIN_FILENO, buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                continue;
            }

            Writen(sockfd, buf, n);
        } else if (FD_ISSET(sockfd, &readset)) {
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof)   break;
                else            err_quit("Server stopped prematurely");
            }

            Write(STDOUT_FILENO, buf, n);
        }
    }

    Close(sockfd);

    exit(EXIT_SUCCESS);
}
