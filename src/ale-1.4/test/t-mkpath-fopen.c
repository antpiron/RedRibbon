#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/fileutils.h"

char *paths[] = {"/tmp/ale-test/test1/test2/file.test", NULL};

int
main(int argc, char *argv[argc])
{
  for (char **path = paths ; *path ; path++)
    {
      FILE* file;

      ERROR_FATAL_FMT(NULL == (file = mkpath_fopen(*path, "w")), "FAIL: mkpath_fopen(\"%s\", \"w\")\n", *path);
      ERROR_ERRNO_FATAL(-1 == fclose(file), "FAIL: unable to close file\n");
      ERROR_ERRNO_FATAL(-1 == access(*path, F_OK|W_OK), "FAIL: access refused\n");
      ERROR_FATAL_FMT(NULL != (file = mkpath_fopen(*path, "wx")), "FAIL: exclusive mkpath_fopen(\"%s\", \"wb\")\n", *path);
    }
            
  return EXIT_SUCCESS;
}
