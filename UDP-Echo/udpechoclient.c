/* Forked UDP Echo client.
   Client reads STDIN, sends UDP packet to the specified host and port,
   waits for feedback and prints feedback on STDOUT

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

        /* There is an issue here: we potentially might accept UDP packet that
           didn't come from 'servaddr'. This might be a random backet from some
           other host or packet from the server that was send from differnt
           interface due to server's routing table.

           The workarounds for the issue are described in Stevens "UNIX network
           programming":
             1) On server side, don't bind socket to the wildcard address but to
             each network interface, i.e. create as many listening sockets as
             there are network interfaces on the node. Then use 'select' to
             multiplex incoming packets and reply: UDP packet would have the IP
             address of a socket it has been sent from.

             or

             2) Get the DNS domain name by IP where UDP datagram is sent and
             received from. Compare these addresses.
           */
        size_t n = Recvfrom(sockfd, buf, MAXLINE-1, 0, NULL, NULL);
        buf[n] = '\0';
        fputs(buf, stdout);
    }

    Close(sockfd);
}
