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

// For socket
#include <sys/types.h>
#include <sys/socket.h>

// sockaddr_in, INADDR_ANY
#include <netinet/in.h>

// write
#include <unistd.h>

// inet_ntop
#include <arpa/inet.h>

// bzero
#include <strings.h>

#define BUFFER_MAX  1024
#define PORT        17

int main()
{
    char buffer[BUFFER_MAX];
    int status;

    int serverfd = socket(PF_INET, SOCK_STREAM, 0);
    if (serverfd == -1) {
        perror("Error openning socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    status = bind(serverfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr_in));
    if (status == -1) {
        perror("Error bind socket");
        exit(EXIT_FAILURE);
    }

    status = listen(serverfd, 5);
    if (status == -1) {
        perror("Error listen socket");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t cliaddrlen = sizeof(cliaddr);

        int clientfd = accept(serverfd, (struct sockaddr *) &cliaddr, &cliaddrlen );
        if (clientfd == -1)
            perror("Error accepting connection");
        else {
            if (inet_ntop(AF_INET, (void *) &cliaddr.sin_addr, buffer, cliaddrlen)) {
                printf("Connection from [%s:%d]\n", buffer, cliaddr.sin_port);

                int n = snprintf(buffer, BUFFER_MAX, "Hello, World!\r\n");
                write(clientfd, buffer, n);
                close(clientfd);
            } else
                perror("ntop error");
        }
    }

    close(serverfd);

    exit(EXIT_SUCCESS);
}
