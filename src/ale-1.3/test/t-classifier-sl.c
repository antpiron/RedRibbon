#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/knn.h"

int
main(int argc, char *argv[argc])
{
  struct sl_node node;
  struct classifier_sl_dc *dc;

  ERROR_FATAL(0 != classifier_sl_init(&node), "FAIL: classifier_sl_init()\n" );

  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 1, 1), "FAIL: classifier_sl_add()\n");
  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 2, 2), "FAIL: classifier_sl_add()\n");

  ERROR_UNDEF_FATAL(-1 == sl_pop(&node, (void**) &dc), "FAIL: sl_pop() returned -1\n");
  ERROR_FATAL_FMT(2 != dc->class, "FAIL: sl_pop() val %u != 2\n", dc->class);
  ERROR_FATAL_FMT(2 != (intptr_t) dc->data, "FAIL: sl_pop() val %"PRIdPTR" != 2\n", (intptr_t) dc->data);

  ERROR_UNDEF_FATAL(-1 == sl_pop(&node, (void**) &dc), "FAIL: sl_pop() returned -1\n");
  ERROR_FATAL_FMT(1 != dc->class, "FAIL: sl_pop() val %u != 1\n", dc->class);
  ERROR_FATAL_FMT(1 != (intptr_t) dc->data, "FAIL: sl_pop() val %"PRIdPTR" != 1\n", (intptr_t) dc->data);

  classifier_sl_destroy(&node);
  
  return EXIT_SUCCESS;
}
