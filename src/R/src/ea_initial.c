#include "config.h"

#include "RedRibbon.h"


int
rrho_initial_population_func(struct rrho_coord *coord, size_t index,
			     struct rrho_rectangle_params_ea *params, void *cls)
{
  struct rrho *rrho = cls;
  const size_t max_n = 100;
  size_t m = (max_n < params->ilen) ? max_n : params->ilen;
  size_t n = (max_n < params->jlen) ? max_n : params->jlen;

  if (0 == index)
    {
      int ret = rrho_rectangle_min(rrho,
				   params->i, params->j, params->i + m, params->j + n,
				   &(struct rrho_rectangle_params){ .m = m, .n = n },
				   params->mode, params->direction,
				   coord);
      if (0 != ret)
	{
	  warning("rrho_initial_population_func returned %d\n", ret);
	  
	  return 0;
	}

      Rprintf("(%zu, %zu)\n", coord->i, coord->j);
      
      return 1;
    }
  else if ( 1 == index && ! (m == params->ilen && n == params->jlen) )
    {
      int ret = rrho_rectangle_min(rrho,
				   params->i, params->j, params->ilen, params->jlen,
				   &(struct rrho_rectangle_params){ .m = m, .n = n },
				   params->mode, params->direction,
				   coord);
      if (0 != ret)
	{
	  warning("rrho_initial_population_func returned %d\n", ret);
	  
	  return 0;
	}
      
      return 1;
    }

  
  return 0;
}
