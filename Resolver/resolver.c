/* Resolve host name to IP address
   This is a simple hostname resolver. The program
   takes hostname as a parameter and makes request
   to system resolver to get a list of IP addresses.
   Both, IPv4 and IPv6 addresses would be return if
   the host has ones.

   Tested on RH Linux 6, Mac OS X 10.11

   Copyright: Volodymyr Tykhoniuk, 2016
   E-mail: volodymyr.tykhoniuk@gmail.com
   */

#include "unet.h"


int main(int argc, char** argv)
{
    if (argc != 2)
        err_quit("Usage: %s <host>", argv[0]);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *result;
    Getaddrinfo(argv[1], NULL, &hints, &result);

    struct addrinfo *res = result;
    do {
        char *ipstr;
        Sock_ntop_host((SA *) res->ai_addr, &ipstr);
        puts(ipstr);
        free(ipstr);
    } while ((res = res->ai_next) != NULL);

    freeaddrinfo(result);

    exit(EXIT_SUCCESS);
}
