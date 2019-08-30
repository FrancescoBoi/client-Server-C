//#include "apue.h"
#include <netdb.h>
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include<unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "../common/error.h"
#include <signal.h>
#define BUFLEN  4096
#define TIMEOUT 10
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 156
#endif

//int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

void print_uptime(int sockfd, struct addrinfo *aip);
void sigalrm(int signo);

int main(int argc, char *argv[])
{
    char str[] ="LD_LIBRARY_PATH=/home/fra/Documents/openCV/openCV/build/lib/:/home/fra/Documents/openCV/poco/instDir/lib/:/home/fra/Documents/openCV/SDL2-2.0.8/instDir/lib";
    putenv(str);
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
   //printf("print uptime called\n");
   int     n, status;
   u_char    buf[BUFLEN];
   size_t read = 0;
   size_t bytesToReceive = BUFLEN;
   size_t total_bytes = 0;
   u_char *img;
   FILE *fp;
   buf[0] = 0;
   printf("Forking...\n");
   if ((fp = popen("/home/fra/Desktop/imgTransferC/childDB/openCV", "w")) == NULL)
   {
       //error
       printf("forkin error\n");
       return;
   }
   if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen)<0)
   {
       err_sys("sendto err");
   }
   //When the timer expires, SIGALRM is generated
   n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL);
   /*if(fwrite(buf, 10, 1, fp)!=1)
   {
       printf("error sending size, exiting program\n");
       return;
   }*/
   total_bytes = atoi((char*)buf);
   if(fwrite(buf, 10, 1, fp)!=1)
   {
       printf("error sending size to client child\n");
       return;
   }
   printf("Converted valueeeee for expected size: %lu\n", total_bytes);
   img = malloc(total_bytes*sizeof(u_char));

   while(read<total_bytes)
   {
       alarm(TIMEOUT);
       //printf("recv again\n");
       n = recvfrom(sockfd, img+read, bytesToReceive, 0, NULL, NULL);
       read += n;// to keep the defensive if later
       /*
       if(fwrite(buf, n, 1, fp)!=1)
       {
           printf("error sending size, exiting program\n");
           return;
       }*/
       bytesToReceive = (total_bytes-read)<BUFLEN ? (total_bytes-read) : BUFLEN;
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
           printf("Entered buf[0] cond\n");
           break;
       }

   }
   printf("total byets read: %ld\n", read);
   size_t sent = 0;
   unsigned int is_sent = 0;
   n = BUFLEN;
   while(sent<total_bytes)
   {
       if((is_sent = fwrite(img+sent, n, 1, fp))!=1)
       {
           printf("error sending size, exiting program\n");
           return;
       }
       //sleep(1);
       sent += is_sent*n;
       n = BUFLEN<(total_bytes-sent) ? BUFLEN:(total_bytes-sent);
       //printf("parent: sent %ld bytes over %ld\n", sent, total_bytes);
   }
   /*if ((n=recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL))<0)
   {
       if (errno !=EINTR)
       {
           alarm(0);
       }
       err_sys("recv error");
   }*/
   printf("Waiting for child...\n");
   pclose(fp);
   printf("reaching end freeing img\n");
   free(img);
   alarm(0);
   //printf("waiting for child to terminate\n");

   //write(STDOUT_FILENO, buf, n);
}

void sigalrm(int signo)
{

}
