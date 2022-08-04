#ifndef __DAEMON_H
#define __DAEMON_H

#include <sys/types.h>

int drop_privileges(uid_t uid, gid_t gid);
int daemon_init(char *pidfile);

#endif
