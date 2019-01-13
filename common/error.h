#ifndef _ERROR_H_
#define _ERROR_H_
#endif
#include<stdarg.h>
void err_ret (const char *fmt, ...);

/*Fatal error related to a system cal1.
* Print a message and terminate*/
void err_sys(const char *fmt,...);

/*Nonfatal error unrelated to a system call.
Error code passed as explict parameter
Print a message and return*/
void err_cont(int error, const char *fmt,...);

/*Fatal error unrelated to a system call
t Error code passed as explict parameter
*Print a message and terminate.*/
void err_exit(int error, const char *fmt, ...);

/*Fatal error related to a system call.
*Print a message, dump core, and terminate*/
void err_dump(const char *fmt,...);


/*Nonfatal error unrelated to a system call.

Print a message and return*/
void err_msg(const char *fmt, ...);

/* Fatal error unrelated to a system call.
* Print a message and terminate*/
void err_quit (const char *fmt, ...);

