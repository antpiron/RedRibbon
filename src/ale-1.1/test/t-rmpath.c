#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/fileutils.h"

#define BASEPATH "/tmp/ale-test/"

int
main(int argc, char *argv[argc])
{
  ERROR_FATAL(-1 == mkpath(BASEPATH"rmpath/p1", S_IRWXU), "FAIL: creating path p1\n");
  ERROR_FATAL(-1 == mkpath(BASEPATH"rmpath/p2", S_IRWXU), "FAIL: creating path p2\n");
  ERROR_FATAL(-1 == rmpath(BASEPATH, "rmpath/p1") && ale_error.type != ERR_ERRNO && errno != ENOTEMPTY, "FAIL: removing path p1\n");
  ERROR_UNDEF_FATAL(0 == access(BASEPATH"rmpath/p1", F_OK), "FAIL: path rmpath/p1 not removed\n");
  ERROR_ERRNO_FATAL( -1 == access(BASEPATH"rmpath", F_OK), "FAIL: path rmpath have been removed prematurely\n");
  ERROR_FATAL(-1 == rmpath(BASEPATH, "rmpath/p2"), "FAIL: removing path p2\n");
  ERROR_UNDEF_FATAL(0 == access(BASEPATH"rmpath", F_OK), "FAIL: path rmpath have not been removed\n" );
 
           
  return EXIT_SUCCESS;
}
