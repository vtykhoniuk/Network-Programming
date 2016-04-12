#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unet.h"

#define PORT        7777

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <IP>", argv[0]);

    int sockfd = Socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    Sock_set_port((SA *) &servaddr, PORT);
    Sock_set_addr((SA *) &servaddr, argv[1]);

    char buf[MAXLINE];
    while ((fgets(buf, MAXLINE, stdin)) != NULL) {
        Sendto(sockfd, buf, strlen(buf), 0, (SA *) &servaddr, sizeof(servaddr));
        size_t n = Recvfrom(sockfd, buf, MAXLINE-1, 0, NULL, NULL);
        buf[n] = '\0';
        fputs(buf, stdout);
    }

    Close(sockfd);
}
