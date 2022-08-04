#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <float.h>

#include "ale/error.h"
#include "ale/knn.h"

int
classifier_sl_add(struct sl_node *list, void *data, unsigned int class)
{
  struct classifier_sl_dc *dc = malloc(sizeof(struct classifier_sl_dc));
  ERROR_UNDEF_RET(NULL == dc, -1);

  
  dc->data = data;
  dc->class = class;

  return sl_push(list, dc);
}

void
classifier_sl_destroy(struct sl_node *list)
{
  classifier_sl_destroy_full(list, NULL);
}

void
classifier_sl_destroy_full(struct sl_node *list, void (*destroy_data)(void *))
{
  struct classifier_sl_dc *dc;

  while ( 0 == sl_pop(list, (void**) &dc) )
    if ( NULL != destroy_data )
      destroy_data(dc->data);

  sl_destroy(list);
}


struct kentry
{
  double distance ;
  unsigned int class;
};

static void
insert_in_k(unsigned int k, struct kentry *sorted[k+1], double distance, unsigned int class)
{
  unsigned int i;
  
  if (distance >= sorted[1]->distance)
    return;
  
  for (i = 1 ; i < k+1 && distance < sorted[i]->distance ; i++)
    sorted[i-1] = sorted[i];

  sorted[0]->distance = distance;
  sorted[0]->class = class;
  sorted[i-1] = sorted[0];
}
  
// Naive classifier unsuitable for large dataset to classify
unsigned int
knn_classifier(unsigned int k, void *data, struct sl_node *learned,
	       double (*distance_func)(void *a, void *b, void *cls), void *cls,
	       unsigned int *score)
{
  // First elem is dummy
  struct kentry kentry_data[k+1];
  struct kentry *sorted[k+1];
  
  for (unsigned int i = 0 ; i < k+1 ; i++)
    {
      sorted[i] = kentry_data+i;
      kentry_data[i].distance = DBL_MAX;
    }

  for (struct sl_node *cur = sl_first(learned) ;
       NULL != cur ;
       cur = sl_next(cur))
    {
      struct classifier_sl_dc *dc = cur->data;
      double distance = distance_func(dc->data, data, cls);
      
      insert_in_k(k, sorted, distance, dc->class);
    }

  unsigned int count_size = 0;
  for (unsigned int i = 1 ; i < k+1 ; i++)
    if (sorted[i]->class > count_size)
      count_size = sorted[i]->class;
  count_size++;
  
  unsigned int count[count_size];
  memset(count, 0, sizeof(count));
  for (unsigned int i = 1 ; i < k+1 ; i++)
    count[sorted[i]->class]++;

  unsigned int max = 0;
  unsigned int class = 0;
  for (unsigned int i = 0 ; i < count_size ; i++)
    if (count[i] > max)
      {
	max = count[i];
	class = i;
      }

  if (NULL != score)
    *score = max;
  
  return class;
}
