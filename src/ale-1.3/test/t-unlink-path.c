#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/fileutils.h"

#define BASEPATH "/tmp/ale-test/"

int
main(int argc, char *argv[argc])
{
  FILE *file;

  ERROR_FATAL(NULL == (file = mkpath_fopen(BASEPATH"unlink-path/p1", "w")), "FAIL: creating file p1\n");
  ERROR_ERRNO_FATAL(-1 == fclose(file), "FAIL: unable to close file");
 
  ERROR_FATAL(NULL == (file = mkpath_fopen(BASEPATH"unlink-path/p2", "w")), "FAIL: creating file p2\n");
  ERROR_ERRNO_FATAL(-1 == fclose(file), "FAIL: unable to close file");

  ERROR_FATAL(-1 == unlink_path(BASEPATH, "unlink-path/p1"),"FAIL: removing file p1\n");
  
  ERROR_UNDEF_FATAL(0 == access(BASEPATH"unlink-path/p1", F_OK), "FAIL: path unlink-path/p1 not removed\n");

  ERROR_UNDEF_FATAL(0 != access(BASEPATH"unlink-path", F_OK), "FAIL: path unlink-path have been removed prematurely\n");

  ERROR_FATAL(-1 == unlink_path(BASEPATH, "unlink-path/p2"), "FAIL: removing file p2\n");

  ERROR_UNDEF_FATAL(0 == access(BASEPATH"unlink-path", F_OK), "FAIL: path unlink-path have not been removed\n");
  
           
  return EXIT_SUCCESS;
}
