#include <stdlib.h>

#include "ale/error.h"
#include "ale/sl_list.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  struct sl_node lst;

  ERROR_UNDEF_FATAL(0 != sl_init(&lst), "FAIL: sl_init()" );
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: lst.next != NULL");
  
  return EXIT_SUCCESS;
}
