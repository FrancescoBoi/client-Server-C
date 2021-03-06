//#include "apue.h"
#include <netdb.h>
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include<unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "../common/error.h"
#define BUFLEN      128
#define MAXSLEEP 128
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 156
#endif
int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

void print_uptime(int sockfd);

int main(int argc, char *argv[])
{
    char *host;
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int     sockfd, err;
    int n;
    if (argc != 2)
        err_quit("usage: ruptime hostname");
    if ((n = sysconf(_SC_HOST_NAME_MAX))<0)
    {
        n = HOST_NAME_MAX;
    }

    if((host = malloc(n)) == NULL)
    {
        printf("malloc error\n");
        exit(1);
    }
    if (gethostname(host, n)<0)
    {
      printf("gethostname error\n");
      exit(1);
    }
    printf("hostname: %s\n", host);
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    //if ((err = getaddrinfo(argv[1], "60185", &hint, &ailist)) != 0)
    if((err = getaddrinfo(host, "ruptime", &hint, &ailist))!=0)
    if ((err = getaddrinfo("127.0.0.1", "60185", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        printf("Trying a connection\n");
        if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
        aip->ai_addr, aip->ai_addrlen)) < 0)
        {
            err = errno;
        }
        else
        {
            print_uptime(sockfd);
            exit(0);
        }
    }
    err_exit(err, "can’t connect to %s", argv[1]);
}


void print_uptime(int sockfd)
{
   int     n;
   char    buf[BUFLEN];
   while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
       write(STDOUT_FILENO, buf, n);
   if (n < 0)
       err_sys("recv error");
}

int connect_retry(int domain, int type, int protocol,
              const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;
    /*
     * Try to connect with exponential backoff.
     */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        printf("Trying\n");
        if ((fd = socket(domain, type, protocol)) < 0)
        {
            return(-1);
        }
        if (connect(fd, addr, alen) == 0)
        {
                /*
                 * Connection accepted.
                 */
            printf("Connected\n");
            return(fd);
        }
        close(fd);
            /*
             * Delay before trying again.
             */
        if (numsec <= MAXSLEEP/2)
            sleep(numsec);
    }
    printf("\n\n");
    return(-1);
}
