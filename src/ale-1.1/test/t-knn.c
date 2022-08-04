#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/knn.h"


double
abs_func(void *a, void *b, void *cls)
{
  intptr_t val = (intptr_t) a - (intptr_t) b ;

  return (val < 0)?-val:val;
}

int
main(int argc, char *argv[argc])
{
  struct sl_node node;
  unsigned int class;
  unsigned int score;
  
  ERROR_FATAL(0 != classifier_sl_init(&node), "FAIL: classifier_sl_init()\n" );

  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 1, 1), "FAIL: classifier_sl_add()\n");
  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 2, 1), "FAIL: classifier_sl_add()\n");
  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 3, 1), "FAIL: classifier_sl_add()\n");
  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 6, 2), "FAIL: classifier_sl_add()\n");
  ERROR_FATAL(0 != classifier_sl_add(&node, (void*) (intptr_t) 7, 2), "FAIL: classifier_sl_add()\n");

  class = knn_classifier(3, (void*) (intptr_t) 3, &node, abs_func, NULL, NULL);
  ERROR_UNDEF_FATAL_FMT(1 != class, "FAIL: knn_classifier(3) %d != 1\n", class);

  class = knn_classifier(3, (void*) (intptr_t) 7, &node, abs_func, NULL, NULL);
  ERROR_UNDEF_FATAL_FMT(2 != class, "FAIL: knn_classifier(7) %d != 2\n", class);

  for (intptr_t i = 0 ; i < 4 ; i++)
    {
      class = knn_classifier(3, (void*) i, &node, abs_func, NULL, &score);
      ERROR_UNDEF_FATAL_FMT(1 != class, "FAIL: knn_classifier(%"PRIdPTR") %d != 1\n", i, class);
      ERROR_UNDEF_FATAL_FMT(score < 2, "FAIL: knn_classifier(%"PRIdPTR") %d < 2\n", i, score);
    }

  for (intptr_t i = 5 ; i < 10 ; i++)
    {
      class = knn_classifier(3, (void*) (intptr_t) i, &node, abs_func, NULL, &score);
      ERROR_UNDEF_FATAL_FMT(2 != class, "FAIL: knn_classifier(%"PRIdPTR") %d != 1\n", i, class);
      ERROR_UNDEF_FATAL_FMT(score < 2, "FAIL: knn_classifier(%"PRIdPTR") %d < 2\n", i, score);
    }
  
  classifier_sl_destroy(&node);
  
  return EXIT_SUCCESS;
}
