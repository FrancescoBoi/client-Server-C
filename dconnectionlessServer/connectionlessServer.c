#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //_SC_HOST_NAME_MAX
#include<string.h>
#include<netdb.h> //Here are defined AF_INET and the others of the family
#include<syslog.h> //LOG_ERR
#include<errno.h> //errno
#include <sys/types.h>
#include <sys/wait.h>

#include"../common/utilities.h"
#include "error.h"

#define BUFLEN 128
#define QLEN 10
#define MAXADDRLEN 256
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 156
#endif

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen);

void serve(int sockfd);
void serve2(int sockfd);
void serveConnectionless(int sockfd);

int main(int argc, char* argv[])
{
    char str[] ="LD_LIBRARY_PATH=/home/fra/Documents/openCV/openCV/build/lib/:/home/fra/Documents/openCV/poco/instDir/lib/:/home/fra/Documents/openCV/SDL2-2.0.8/instDir/lib";
    putenv(str);
    printf("entered main\n");
    /*The C data structure used to represent addresses and hostnames within
    the networking API is the following*/
    struct addrinfo *ailist, *aip, hint;
    int sockfd, err, n;
    char *host;
    if (argc != 1)
    {
        printf("usage: ruptimed\n");
        exit(1);
    }
    /*
      sysconf: get configuration information at run time
      HOST_NAME_MAX - _SC_HOST_NAME_MAX
      Maximum length of a hostname, not including the terminating
      null byte, as returned by gethostname(2).  Must not be less
      than _POSIX_HOST_NAME_MAX (255).*/
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
    printf("Daemonizing\n");
    daemonize("ruptimed");
    //printf("Daemonized\n");
    /*       The hints argument points to an addrinfo structure that specifies
       criteria for selecting the socket address structures returned in the
       list pointed to by res.  If hints is not NULL it points to an
       addrinfo structure whose ai_family, ai_socktype, and ai_protocol
       specify criteria that limit the set of socket addresses returned by
       getaddrinfo(), as follows:

       ai_family   This field specifies the desired address family for the
                   returned addresses.  Valid values for this field include
                   AF_INET and AF_INET6.  The value AF_UNSPEC indicates that
                   getaddrinfo() should return socket addresses for any
                   address family (either IPv4 or IPv6, for example) that
                   can be used with node and service.

       ai_socktype This field specifies the preferred socket type, for exam‐
                   ple SOCK_STREAM or SOCK_DGRAM.  Specifying 0 in this
                   field indicates that socket addresses of any type can be
                   returned by getaddrinfo().

       ai_protocol This field specifies the protocol for the returned socket
                   addresses.  Specifying 0 in this field indicates that
                   socket addresses with any protocol can be returned by
                   getaddrinfo().

       ai_flags    This field specifies additional options, described below.
                   Multiple flags are specified by bitwise OR-ing them
                   together.

       All the other fields in the structure pointed to by hints must con‐
       tain either 0 or a null pointer, as appropriate.*/
    memset(&hint, 0, sizeof(hint)); //set to 0 all bytes
    /*If hints.ai_flags includes the AI_CANONNAME flag, then the ai_canon‐
       name field of the first of the addrinfo structures in the returned
       list is set to point to the official name of the host.*/
    //hint.ai_flags = AI_CANONNAME;
    hint.ai_flags |= AI_PASSIVE;
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    /*If the AI_PASSIVE flag is specified in hints.ai_flags, and node is
       NULL, then the returned socket addresses will be suitable for
       binding a socket that will accept(2) connections.  The returned
       socket address will contain the "wildcard address" (INADDR_ANY for
       IPv4 addresses, IN6ADDR_ANY_INIT for IPv6 address).  The wildcard
       address is used by applications (typically servers) that intend to
       accept connections on any of the host's network addresses.  If node
       is not NULL, then the AI_PASSIVE flag is ignored.

       If the AI_PASSIVE flag is not set in hints.ai_flags, then the
       returned socket addresses will be suitable for use with connect(2),
       sendto(2), or sendmsg(2).  If node is NULL, then the network address
       will be set to the loopback interface address (INADDR_LOOPBACK for
       IPv4 addresses, IN6ADDR_LOOPBACK_INIT for IPv6 address); this is used
       by applications that intend to communicate with peers running on the
       same host.

       Service sets the port in each returned address structure.  If this
       argument is a service name (see services(5)), it is translated to the
       corresponding port number.  This argument can also be specified as a
       decimal number, which is simply converted to binary.  If service is
       NULL, then the port number of the returned socket addresses will be
       left uninitialized.  If AI_NUMERICSERV is specified in hints.ai_flags
       and service is not NULL, then service must point to a string contain‐
       ing a numeric port number.  This flag is used to inhibit the invoca‐
       tion of a name resolution service in cases where it is known not to
       be required.

       Either node or service, but not both, may be NULL.*/
    if((err = getaddrinfo(NULL, "60185", &hint, &ailist))!=0)
    {
        printf("Error line 129 %d\n", err);
        syslog(LOG_ERR, "ruptimed: getaddrinfo error %s", gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip!=NULL; aip = aip->ai_next)
    {
        if ((sockfd = initserver(SOCK_DGRAM, aip->ai_addr, aip->ai_addrlen, QLEN))>=0)
        {
            //printf("starting to serve\n");
            serveConnectionless(sockfd);
            printf("Exiting \n");
            exit(0);
        }
    }
    exit(1);
}


void serveConnectionless(int sockfd)
{
    int n;
    socklen_t alen;
    FILE *fp;
    char buf[BUFLEN];
    char abuf[MAXADDRLEN];
    struct sockaddr *addr = (struct sockaddr*)abuf;
    for(;;)
    {
        alen = MAXADDRLEN;
        if((n=recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen))<0)
        {
            syslog(LOG_ERR, "ruptimed: recvfrom error %s", strerror(errno));
            exit(1);
        }
        /**pg. 542 Since a common operation is to create a pipe to another process
        to either read its output or write its input stdio has provided popen and
        pclose: popen creates pipe, close the unused ends of the pipe,
        forks a child and call exec to execute cmdstr and
        returns a file pointer (connected to std output if "r", to stdin if "w").
        pclose closes the stream, waits for the command to terminate*/
        if ((fp = popen("/home/fra/Desktop/openCV", "r")) == NULL)
        {
            /*sprintf copy the string passed as second parameter inside buf*/
            sprintf(buf, "error: %s\n", strerror(errno));
            /*pag 610. send is similar to write. send(int sockfd, const void *buf, size_t nbytes, it flags)*/
            //send(clfd, buf, strlen(buf),0);
            sendto(sockfd, buf, strlen(buf),0, addr, alen);
        }
        else
        {
            /*get data from the pipe that reads created to exec /usr/bin/uptime */
            while(fgets(buf, BUFLEN, fp)!=NULL)
            {
                sendto(sockfd, buf, strlen(buf),0, addr, alen);
            }
            /*see popen pag. 542*/
            pclose(fp);
        }
        //close(clfd);
    }
}


void serve(int sockfd)
{
    int clfd;
    FILE *fp;
    char buf[BUFLEN];
    set_cloexec(sockfd);
    for(;;)
    {
        /*After listen, the socket can receive connect requests. accept
        retrieves a connect request and converts it into a connection.
        The file returned by accept is a socket descriptor connected to the client that
        called connect, haing the same coket type and family type. The original
        soket remains available to receive otherconneion requests. If we don't care
        about client's identity we can set the second (struct sockaddr *addr)
        and third parameter (socklen_t *len) to NULL*/
        if((clfd = accept(sockfd, NULL, NULL))<0)
        {
            /*This generates a log mesage.
            syslog(int priority, const char *fformat,...)
            priority is a combination of facility and level. Levels are ordered from highest to lowest:
            LOG_EMERG: emergency system unusable
            LOG_ALERT: condiotin that must be fied immediately
            LOG_CRIT: critical condition
            LOG_ERR: error condition
            LOG_WARNING
            LOG_NOTICE
            LOG_INFO
            LOG_DEBUG
            format and other arguements are passed to vsprintf function forf formatting.*/
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
            exit(1);
        }
        /* set the FD_CLOEXEC file descriptor flag */
        /*it causes the file descriptor to be automatically and atomically closed
         when any of the exec family function is called*/
        set_cloexec(clfd);
        /**pg. 542 Since a common operation is to create a pipe to another process
        to either read its output or write its input stdio has provided popen and
        pclose: popen creates pipe, close the unused ends of the pipe,
        forks a child and call exec to execute cmdstr and
        returns a file pointer (connected to std output if "r", to stdin if "w").
        pclose closes the stream, waits for the command to terminate*/
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            /*sprintf copy the string passed as second parameter inside buf*/
            sprintf(buf, "error: %s\n", strerror(errno));
            /*pag 610. send is similar to write. send(int sockfd, const void *buf, size_t nbytes, it flags)*/
            send(clfd, buf, strlen(buf),0);
        }
        else
        {
            /*get data from the pipe that reads created to exec /usr/bin/uptime */
            while(fgets(buf, BUFLEN, fp)!=NULL)
            {
                /* clfd is returned by accept and it is a socket descriptor
                connected to the client that called connect*/
                send(clfd, buf, strlen(buf), 0);
            }
            /*see popen pag. 542*/
            pclose(fp);
        }
        close(clfd);
    }
}



void serve2(int sockfd)
{
    int clfd, status;
    pid_t pid;
    //FILE *fp;
    //char buf[BUFLEN];
    set_cloexec(sockfd);
    for(;;)
    {
        /*After listen, the socket can receive connect requests. accept
        retrieves a connect request and converts it into a connection.
        The file returned by accept is a socket descriptor connected to the client that
        called connect, haing the same coket type and family type. The original
        soket remains available to receive otherconneion requests. If we don't care
        about client's identity we can set the second (struct sockaddr *addr)
        and third parameter (socklen_t *len) to NULL*/
        if((clfd = accept(sockfd, NULL, NULL))<0)
        {
            /*This generates a log mesage.
            syslog(int priority, const char *fformat,...)
            priority is a combination of facility and level. Levels are ordered from highest to lowest:
            LOG_EMERG: emergency system unusable
            LOG_ALERT: condiotin that must be fied immediately
            LOG_CRIT: critical condition
            LOG_ERR: error condition
            LOG_WARNING
            LOG_NOTICE
            LOG_INFO
            LOG_DEBUG
            format and other arguements are passed to vsprintf function forf formatting.*/
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
            exit(1);
        }
        if ((pid = fork())<0)
        {
            syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));
            exit(1);
        }
        else if (pid == 0) //CHILD
        {
            /*The parent called daemonize (Figure 13.1), so STDIN_FILENO,
             STDOUT_FILENO, and STDERR_FILENO are already open to /dev/NULL
             Thus the call to close does not need to be protected by checks that
             clfd  is not already equal to one of these values. */
             if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                dup2(clfd, STDERR_FILENO)!= STDERR_FILENO)
            {
                syslog(LOG_ERR, "ruptimed: unexpected error\n");
                exit(1);
            }
            close(clfd);
            execl("/usr/bin/uptime", "uptime", (char*)0);
            syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s", strerror(errno));
        }
        else
        {
            //parent
            close(clfd);
            waitpid(pid, &status, 0);
        }
    }
}


/*struct sockaddr *ai_addr;*/
/*socklen_t        ai_addrlen;*/
/*  initserver(SOCK_STREAM, aip->ai_addr,               aip->ai_addrlen, QLEN))>=0)*/
int initserver(int type,   const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd, err;
    int reuse = 1;
    if ((fd = socket(addr->sa_family, type, 0))<0)
    {
        return (-1);
    }
    /*Use this constant as the level argument to getsockopt or setsockopt to
    manipulate the socket-level options described in this section*/
    /*       SO_REUSEADDR
              Indicates that the rules used in validating addresses supplied
              in a bind(2) call should allow reuse of local addresses.  For
              AF_INET sockets this means that a socket may bind, except when
              there is an active listening socket bound to the address.
              When the listening socket is bound to INADDR_ANY with a spe‐
              cific port then it is not possible to bind to this port for
              any local address.  Argument is an integer boolean flag.*/
    /*TCP implementation does not allow to bind the same address until a
    timer expires, usually on the order of several minutes. This is a
    problem when restarting the server. SO_REUSEADDR allows us to bypass
    this restriction.*/
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int))<0)
    {
        goto errout;
    }
    if(bind(fd, addr, alen)<0)
    {
        goto errout;
    }
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if(listen(fd, qlen)<0)
        {
            goto errout;
        }
    }
    return fd;
    errout:
        err = errno;
        close (fd);
        errno = err;
        return(-1);
}
