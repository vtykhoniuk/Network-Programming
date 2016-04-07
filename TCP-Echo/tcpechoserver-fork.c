/* Forked TCP Echo server.
   It waits for incoming tcp connections on port 777 and echoes back everything
   whatever client sent. For each incoming request new process has been created
   using 'fork' syscall.

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

// getpid
#include <unistd.h>

// waitpid
#include <sys/wait.h>

#include "unet.h"

#define PORT        7777

void process_client(int clientfd);
void sig_child(int signo);

int main()
{
    int serverfd;

    serverfd = Socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, '\0', sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    Sock_set_wild((SA *) &servaddr);
    Sock_set_port((SA *) &servaddr, PORT);

    Bind(serverfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(serverfd, 5);

    Signal(SIGCHLD, sig_child);

    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t cliaddrlen = sizeof(cliaddr);
        int clientfd;

        /* Since we explicitly set SA_RESTART flag in Signal wraper
           we don't care about interrupted syscalls (EINTR error)
           */
        clientfd = Accept(serverfd, (SA *) &cliaddr, &cliaddrlen);

        /* Though, for systems that don't support SA_RESTART we would need to
           use the following code instead of Accept wrapper, to correctly
           process EINTR error */
        /*
        if ((clientfd = accept(serverfd, (SA *) &cliaddr, &cliaddrlen)) < 0) {
            if (errno == EINTR) continue;
            else err_sys("accept error");
        }
        */

        pid_t childpid;
        if ((childpid = Fork()) == 0) {
            // That's a child branch

            /* Close the descriptor we don't need anymore */
            Close(serverfd);

            /* Logging */
            char *cliaddrstr = Sock_ntop((SA *) &cliaddr);
            printf("Child [%d]: Connection from [%s]\n", getpid(), cliaddrstr);
            free(cliaddrstr);

            process_client(clientfd);
            Close(clientfd);

            printf("Child [%d]: done processing, exiting...\n", getpid());
            exit(EXIT_SUCCESS);
        }

        Close(clientfd);
    }

    Close(serverfd);

    exit(EXIT_SUCCESS);
}

void process_client(int clientfd)
{
    char buf[MAXLINE];
    size_t n;

    while ((n = Readline(clientfd, buf, MAXLINE)) > 0)
        Writen(clientfd, buf, n);
}

void sig_child(int signo)
{
    puts("Got SIGCHLD signal, processing zombies...");

    pid_t childpid;
    while ((childpid = waitpid(-1, NULL, WNOHANG)) > 0)
        printf("Child [%d] has been utilized\n", childpid);

    if (childpid < 0 && errno != ECHILD)
        err_quit("waitpid error");
}
