#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"

#define TEST(test,msg)						\
  do								\
    {								\
      if (test)							\
	{							\
	  fprintf(stderr, "%s\n", (msg));			\
	  exit(EXIT_FAILURE);					\
	}							\
    }								\
  while (0)


int
test_error_undef_ret()
{
  ERROR_UNDEF_RET(1, -1);

  return 0;
}

int
test_error_errno_ret()
{
  errno = EACCES;
  ERROR_ERRNO_RET(1, -1);

  return 0;
}

int
test_error_gai_ret()
{
  ERROR_GAI_RET(1, -1);

  return 0;
}

int
test_error_ferror_ret()
{
  ERROR_FERROR_RET(1, -1);

  return 0;
}

int
test_error_custom_ret()
{
  ERROR_CUSTOM_RET(1, 123, -1);

  return 0;
}

int
test_error_ret()
{
  ERROR_RET(-1 == test_error_errno_ret(), -1);
  
  return 0;
}

int
main(int argc, char *argv[argc])
{

  TEST(-1 != test_error_undef_ret() || ale_error.type != ERR_UNDEF, "FAIL: ERROR_UNDEF_RET()");
  TEST(-1 != test_error_errno_ret() || ale_error.type != ERR_ERRNO || errno != EACCES || ale_error.errnum != EACCES, 
       "FAIL: ERROR_ERRNO_RET()");
  TEST(-1 != test_error_gai_ret() || ale_error.type != ERR_GAI, "FAIL: ERROR_GAI_RET()");
  TEST(-1 != test_error_ferror_ret() || ale_error.type != ERR_FERROR, "FAIL: ERROR_FERROR_RET()");
  TEST(-1 != test_error_custom_ret() || ale_error.type != ERR_CUSTOM || ale_error.errnum != 123 , 
       "FAIL: ERROR_CUSTOM_RET()");

  TEST(-1 != test_error_ret() || ale_error.type != ERR_ERRNO || errno != EACCES || ale_error.errnum != EACCES, 
       "FAIL: ERROR_RET()");

  return EXIT_SUCCESS;
}
