/* Iterative TCP Time server.
   It waits for incoming tcp connections on port 17 and returns nothing more but
   current time. It processes incoming connections iteratively one by one, using
   single process. No additional libs are used: just plain UNIX network interface
   and generic libc.

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

// memset
#include <string.h>

#include <time.h>

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
    memset(&servaddr, '\0', sizeof(servaddr));
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

                time_t t = time(NULL);
                struct tm tm;
                localtime_r(&t, &tm);
                size_t n = strftime(buffer, BUFFER_MAX, "%F %T\n", &tm);
                write(clientfd, buffer, n);

                close(clientfd);
            } else
                perror("ntop error");
        }
    }

    close(serverfd);

    exit(EXIT_SUCCESS);
}
