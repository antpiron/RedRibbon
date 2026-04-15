#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdarg.h>

FILE* process_popenvp(const char *pathname, char *const argv[]);
FILE* process_popenp(const char *pathname, ...);

#endif
