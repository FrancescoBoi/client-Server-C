#ifndef _UTILITIES_
#define _UTILITIES_
#endif

int daemonize(const char *cmd);
/*The function set the FD_CLOEXEC flag of the file descriptor already open that
is passed to as parameter. FD_CLOEXEC causes the file descriptor to be
automatically and atomically closed when any of the exec family function is
called*/
int set_cloexec(int fd);
