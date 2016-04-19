/* TCP Echo Client
   A simple TCP-Echo client. The program reads a line from
   stdin and sends it to the Echo server on TCP port defined
   in PORT macro. Client waits for a feedback from server then
   and prints it out to stdout
   */

#include "unet.h"

#define PORT        7777

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <IP>", argv[0]);

    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    Sock_set_port((SA *) &servaddr, PORT);
    Sock_set_addr((SA *) &servaddr, argv[1]);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    char buf[MAXLINE];
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        Writen(sockfd, buf, strlen(buf));

        /* Since we expect to work with echo server, there
           should always be a response. If nothing comes back
           then something went wrong on a server side
           */
        if (Readline(sockfd, buf, MAXLINE) == 0)
            err_quit("Server stops prematurely");

        Fputs(buf, stdout);
    }

    if (ferror(stdin))
        err_sys("fgets error");

    Close(sockfd);

    exit(EXIT_SUCCESS);
}
