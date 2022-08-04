#include <unistd.h>

#include "ale/process.h"
#include "ale/error.h"
#include "ale/vector.h"

FILE*
process_popenvp(const char *pathname, char *const argv[])
{
  int pipefd[2];
  pid_t pid;
  FILE *file;

  ERROR_ERRNO_RET(-1 == pipe(pipefd), NULL);
  ERROR_ERRNO_GOTO(-1 == (pid = fork()), ERROR_FORK);

  // child
  if (0 == pid)
    {
      close(pipefd[0]);
      ERROR_ERRNO_FATAL(-1 == dup2(pipefd[1], STDOUT_FILENO),
			"process_openvp(): dup2 failed.\n");
      ERROR_ERRNO_FATAL(-1 == execvp(pathname, argv),
			"process_openvp(): execv failed.\n");
      _exit(EXIT_SUCCESS); // never here
    }

  // father
  file = fdopen(pipefd[0],"r");
  ERROR_ERRNO_GOTO(NULL == file, ERROR_FDOPEN);
  close(pipefd[1]);
  
  return file;
  
 ERROR_FDOPEN:
 ERROR_FORK:
  close(pipefd[0]);
  close(pipefd[1]);
  errno = ale_error.errnum;
  ERROR_RET(1, NULL);
}

VECTOR_INIT(ptr,char*)
 
FILE*
process_popenp(const char *pathname, ...)
{
  va_list ap;
  size_t len;
  char *ptr = (char*) pathname;
  struct vector_ptr argv;
  
  vector_ptr_init(&argv);
  
  va_start(ap, pathname); 
  for (len = 0 ; NULL != ptr ; len++)
    {
      ptr = va_arg(ap, char*);
      vector_ptr_set(&argv, len, ptr);
    }
  va_end(ap);

  return process_popenvp(pathname, argv.data);
}
