#if HAVE_CONFIG_H
# include <config.h>
#endif

// #define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ale/error.h"

int
drop_privileges(uid_t uid, gid_t gid)
{
  ERROR_ERRNO_RET( -1 == setregid(gid, gid), -1);
  ERROR_ERRNO_RET( -1 == setreuid(uid, uid), -1);

  return 0;
}

static int
write_pid(char *pidfile)
{
  pid_t pid = getpid();
  FILE *file;

  ERROR_ERRNO_RET( NULL == (file = fopen(pidfile, "w")), -1);

  fprintf(file, "%d", pid);

  ERROR_ERRNO_RET( 0 != fclose(file), -1 );
  
  return 0;
}

/* Stevens Advanced Unix Programming in the UNIX Environment */
int
daemon_init(char *pidfile)
{
  pid_t pid;
  int ret;

  pid = fork();
  
  ERROR_ERRNO_RET(pid < 0, -1);

  if (pid != 0)
    exit(EXIT_SUCCESS);

  setsid();
  ERROR_ERRNO_RET(-1 == chdir("/"), -1);
  umask(0);

  ERROR_RET( (ret = write_pid(pidfile)) < 0, ret);

  return 0;
}
