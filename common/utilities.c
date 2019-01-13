#include "utilities.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/time.h>//getrlimit
#include <sys/resource.h>//getrlimit
#include <signal.h> //sigempyset , asigcation (umask?)
#include <sys/resource.h>
#include <fcntl.h> //O_RDWR
#include <stdarg.h>

#include "error.h"

/*The function creates a daemon*/
int daemonize(const char *cmd)
{
    int fd0, fd1, fd2;
    unsigned int i;
    pid_t pid;
    struct rlimit       rl;
    struct sigaction    sa;
    /* Clear file creation mask.*/
    umask(0);
    /* Get maximum number of file descriptors. */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        err_quit("%s: can’t get file limit", cmd);
    }
    /* Become a session leader to lose controlling TTY. */
    if ((pid = fork()) < 0)
    {
        err_quit("%s: can’t fork", cmd);
    }
    else if (pid != 0) /* parent */
    {
        exit(0); //the parent will exit
    }
    setsid();
    /* Ensure future opens won’t allocate controlling TTYs. */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        err_quit("%s: can’t ignore SIGHUP", cmd);
    }
    if ((pid = fork()) < 0)
    {
        err_quit("%s: can’t fork", cmd);
    }
    else if (pid != 0) /* parent */
    {
        exit(0);
    }
    /*
    * Change the current working directory to the root so
    * we won’t prevent file systems from being unmounted.
    */
    if (chdir("/") < 0)
    {
        err_quit("%s: can’t change directory to /", cmd);
    }
    /* Close all open file descriptors. */
    if (rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }
    printf("closing file descriptors\n");
    for (i = 0; i < rl.rlim_max; i++)
    {
        close(i);
    }
    /* Attach file descriptors 0, 1, and 2 to /dev/null.*/
    //printf not working
    /*printf("closed all file descriptors for daemonizing\n");*/
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    /* Initialize the log file. Daemons do not have a controlling terminal so
    they can't write to stderror. We don't want them to write to the console device
    because on many workstations the control device runs a windowing system. They can't
    write on separate files either. A central daemon error-logging facility is required.
    This is the BSD. 3 ways to generate log messages:
    1) kernel routines call the log function. These messages can be read from /dev/klog
    2) Most user processes (daemons) call syslog to generate log messages. This causes
    messages to be sent to the UNIX domain datagram socket /dev/log
    3) A user process on this host or on other host connected to this with TCP/ID
    can send log messages to UDP port 514. Explicit network programmin is required
    (it is not managed by syslog.
    The syslogd daemon reads al three of log messages.

    openlog is optional since if not called, syslog calls it. Also closelog is optional
    openlog(const char *ident, int option, int facility)
    It lets us specify ident that is added to each logmessage. option is a bitmask:
        LOG_CONS tells that if the log message can't be sent to syslogd via UNIX
        domain datagram, the message is written to the console instead.
    facility lets the configuration file specify that messages from different
    facilities are to be handled differently. It can be specified also in the 'priority'
    argument of syslog. LOG_DAEMON is for system deamons
    */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
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
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
    return 0;
}

/*The function set the FD_CLOEXEC flag of the file descriptor already open that
is passed to as parameter. FD_CLOEXEC causes the file descriptor to be
automatically and atomically closed when any of the exec family function is
called*/
int set_cloexec(int fd)
{
    int val;
    /* retrieve the flags of the file descriptor */
    if((val = fcntl(fd, F_GETFD, 0))<0)
    {
        return -1;
    }
    /* set the FD_CLOEXEC file descriptor flag */
    /*it causes the file descriptor to be automatically and atomically closed
     when any of the exec family function is called*/
    val |= FD_CLOEXEC;
    return (fcntl(fd, F_SETFD, val));
}
