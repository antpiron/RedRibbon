#ifndef __KNN_H
#define __KNN_H

#include <ale/sl_list.h>

struct classifier_sl_dc
{
  unsigned int class;
  void *data;
};

#define classifier_sl_init sl_init
void classifier_sl_destroy(struct sl_node *list);
void classifier_sl_destroy_full(struct sl_node *list, void (*destroy_data)(void *));
int classifier_sl_add(struct sl_node *list, void *data, unsigned int class);

unsigned int knn_classifier(unsigned int k, void *data, struct sl_node *learned,
			    double (*distance_func)(void *a, void *b, void *cls), void *cls,
			    unsigned int *score);

#endif
