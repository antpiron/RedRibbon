#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"
#include "ale/stringutils.h"

#define GZFILE "plop.gz"
#define NOTGZFILE "t-fileutils-isgzip.c"

int
main(int argc, char *argv[argc])
{
  int ret;
  
  ERROR_FATAL_FMT(! is_gzip(GZFILE), "FAIL: is_gzip(%s)", GZFILE);
  ERROR_FATAL_FMT(is_gzip(NOTGZFILE), "FAIL: is_gzip(%s)", NOTGZFILE);

  return EXIT_SUCCESS;
}
