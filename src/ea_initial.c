#include <config.h>

#include "RedRibbon.h"


int
rrho_initial_population_func(struct rrho_coord *coord, size_t index,
			     struct rrho_rectangle_params_ea *params, void *cls)
{
  const size_t max_n = 100;
  size_t m = (max_n < params->ilen) ? max_n : params->ilen;
  size_t n = (max_n < params->jlen) ? max_n : params->jlen;

  if (index < m && index < n)
    {
      size_t term = floor(index * (params->ilen / (double) m));
      coord->i = params->i + term;
      if (params->direction < 0)
	coord->i = params->i + params->ilen - 1 - term;
      coord->j = params->j + floor(index * (params->jlen / (double) n));

      return 1;
    }

  
  return 0;
}
