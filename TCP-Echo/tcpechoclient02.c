/* TCP Echo Client
   A simple TCP-Echo client. The program reads a line from
   stdin and sends it to the Echo server on TCP port defined
   in PORT macro. Client waits for a feedback from server then
   and prints it out to stdout

   I/O multiplexing using select syscall is used
   */

// exit, free
#include <stdlib.h>

// memset
#include <string.h>

// STDIN_FILENO
#include <unistd.h>

#include "unet.h"

#define PORT        7777

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <IP>", argv[0]);

    char buf[MAXLINE];
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    Sock_set_port((SA *) &servaddr, PORT);
    Sock_set_addr((SA *) &servaddr, argv[1]);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    for (;;) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(STDIN_FILENO, &readset);
        FD_SET(sockfd, &readset);

        Select(sockfd+1, &readset, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &readset)) {
            if (fgets(buf, MAXLINE, stdin) == NULL)
                break;

            Writen(sockfd, buf, strlen(buf));
        } else if (FD_ISSET(sockfd, &readset)) {
            if (Readline(sockfd, buf, MAXLINE) == 0)
                err_quit("Server stopped prematurely");

            Fputs(buf, stdout);
        }
    }

    Close(sockfd);

    exit(EXIT_SUCCESS);
}