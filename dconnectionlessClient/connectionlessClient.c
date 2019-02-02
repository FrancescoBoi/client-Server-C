//#include "apue.h"
#include <netdb.h>
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include<unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "../common/error.h"
#include <signal.h>
#define BUFLEN  128
#define TIMEOUT 10
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 156
#endif

//int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

void print_uptime(int sockfd, struct addrinfo *aip);
void sigalrm(int signo);

int main(int argc, char *argv[])
{

    //char *host;
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int     sockfd, err;
    int n;
    struct sigaction sa;
    char *host;

    if (argc != 2)//not needed any more but keep it for consistency
        //err_quit("usage: ruptime hostname");
        printf("no argument passed but thats ok cuz im using 127.0.01 anyway\n");
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

    sa.sa_handler = sigalrm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    //third argument if not NULL would return the old handler
    if (sigaction(SIGALRM, &sa, NULL)<0)
    {
        err_sys("sigaction err");
    }
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    //if ((err = getaddrinfo(argv[1], "60185", &hint, &ailist)) != 0)
    if ((err = getaddrinfo("127.0.0.1", "60185", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        printf("Trying a connection\n");
        /*if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
        aip->ai_addr, aip->ai_addrlen)) < 0)*/
        if((sockfd = socket(aip->ai_family, SOCK_DGRAM,0))<0)
        {
            err = errno;
        }
        else
        {
            print_uptime(sockfd, aip);
            exit(0);
        }
    }
    fprintf(stderr, "can't contact \"127.0.0.1\": %s\n", strerror(err));
    err_exit(err, "can’t connect to %s", argv[1]);
}


void print_uptime(int sockfd, struct addrinfo *aip)
{
   int     n;
   char    buf[BUFLEN];
   buf[0] = 0;

   if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen)<0)
   {
       err_sys("sendto err");
   }
   //When the timer expires, SIGALRM is generated

   do
   {
       alarm(TIMEOUT);
       //printf("recv again\n");
       n =recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL);
       printf("n = %d\n", n);
       if (n<0)
       {
           if (errno !=EINTR)
           {
               alarm(0);
           }
           err_sys("recv error");
           break;
       }
       if (buf[0]=='\0')//null char
       {
           //printf("n=%d Client exited cleanly: %d\n", n, (int)(buf[0]) );
           break;
       }
       write(STDOUT_FILENO, buf, n);

   }while(n>0);
   /*if ((n=recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL))<0)
   {
       if (errno !=EINTR)
       {
           alarm(0);
       }
       err_sys("recv error");
   }*/
   alarm(0);
   write(STDOUT_FILENO, buf, n);
}

void sigalrm(int signo)
{

}
