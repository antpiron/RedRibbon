#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ale/error.h"
#include "ale/sl_list.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  struct sl_node lst;
  struct sl_node *node;
  intptr_t val;

  ERROR_UNDEF_FATAL(0 != sl_init(&lst), "FAIL: sl_init()" );
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: lst.next != NULL");

  for (intptr_t i = 0 ; i < 50 ; i+=2)
    ERROR_FATAL(-1 == sl_insert(&lst, i, (void*) i), "FAIL: sl_insert()");

  node = sl_first(&lst);
  for (intptr_t i = 0 ; NULL != node && i < 50 ; i+=2, node = sl_next(node))
    {
      val = (intptr_t) node->data;
      ERROR_UNDEF_FATAL_FMT(val != i, "FAIL: sl_insert() val %"PRIdPTR" != %"PRIdPTR"\n", val, i);
    }

  for (intptr_t i = 1 ; i < 50 ; i+=2)
    ERROR_FATAL(-1 == sl_insert(&lst, i, (void*) i), "FAIL: sl_insert()");

  node = sl_first(&lst);
  for (intptr_t i = 0 ; NULL != node && i < 50 ; i++, node = sl_next(node))
    {
      val = (intptr_t) node->data;
      ERROR_UNDEF_FATAL_FMT(val != i, "FAIL: sl_insert() val %"PRIdPTR" != %"PRIdPTR"\n", val, i);
    }

  val = (intptr_t) sl_remove(&lst, 25);
  ERROR_UNDEF_FATAL_FMT(val != 25, "FAIL: sl_remove() %"PRIdPTR" != 25\n", val);
  node = sl_first(&lst);
  for (intptr_t i = 0 ; NULL != node && i < 50 ; i++, node = sl_next(node))
    {
      val = (intptr_t) node->data;
      if (i < 25)
	ERROR_UNDEF_FATAL_FMT(val != i, "FAIL: sl_remove() val %"PRIdPTR" != %"PRIdPTR"\n", val, i);
      else
	ERROR_UNDEF_FATAL_FMT(val != i+1, "FAIL: sl_remove() val %"PRIdPTR" != %"PRIdPTR"\n", val, i+1);
    }

  node = sl_nth(&lst, 25);
  ERROR_UNDEF_FATAL(NULL == node, "FAIL: sl_nth()\n");
  val = (intptr_t) node->data;
  ERROR_UNDEF_FATAL_FMT(val != 26, "FAIL: sl_remove() val %"PRIdPTR" != 26\n", val);

  sl_destroy(&lst);
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: sl_destroy()\n");
  
  return EXIT_SUCCESS;
}
