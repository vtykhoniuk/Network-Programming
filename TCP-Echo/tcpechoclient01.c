/* TCP Echo Client
   A simple TCP-Echo client. The program reads a line from
   stdin and sends it to the Echo server on TCP port defined
   in PORT macro. Client waits for a feedback from server then
   and prints it out to stdout
   */

// exit, free
#include <stdlib.h>

// memset
#include <string.h>

#include "unet.h"

#define PORT        7777

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <IP>", argv[0]);

    int sockfd = Socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(struct sockaddr_in));

    servaddr.sin_family = AF_INET;
    Sock_set_port((SA *) &servaddr, PORT);
    Inet_pton(servaddr.sin_family, argv[1], &servaddr);

    Connect(sockfd, (SA *) &servaddr, sizeof(struct sockaddr_in));

    Close(sockfd);

    exit(EXIT_SUCCESS);
}
