#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <omp.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  struct rrho rrho;
  struct rrho_result res;
  double diff, rate;
  struct timespec st, et;
  long double pvalue = 0;
  struct rrho_result rrho_res;
  struct rrho_coord rrho_coord;
  struct rrho_permutation_result perm_res;
  int num_threads = omp_get_max_threads();
  int ret;
  FILE *file;
  struct stats_permutation permutation;
 
  printf("%s: \n", argv[0]);

  ret = mkdir(".perf", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  ERROR_ERRNO_FATAL(ret < 0 && errno != EEXIST, "mkdir() failed.\n");

  file = fopen(".perf/p-rrho-permutation-ea.tsv", "w");
  ERROR_ERRNO_FATAL(NULL == file, "fopen() failed.\n");
  fprintf(file, "n.threads\tn\ttime\n");
  
  for (size_t iter = 1 ; iter <= 32 ; iter *= 2)
    {
      size_t vec_n = iter * 5000;
      size_t vec_perc = 0.01 * vec_n;
      double *a = malloc(vec_n * sizeof(double));
      double *b = malloc(vec_n * sizeof(double));
      size_t m = ceil(sqrt(vec_n)), n = ceil(sqrt(vec_n));
      struct rrho_rectangle_params_ea params_ea;
      
      printf("RRHO for vectors of n = %6zu elements, n.threads = %3d: ", vec_n, num_threads);
	
      for (ssize_t i = 0 ; i < vec_n ; i++)
	{
	  if ( i < vec_perc)
	    {
	      a[i] = -i-1; // stats_unif_std_rand();
	      b[i] = -i-1; // stats_unif_std_rand();
	    }
	  else
	    {
	      a[i] = stats_unif_std_rand();
	      b[i] = stats_unif_std_rand();	      
	    }
	}
      
      rrho_init(&rrho, vec_n, a, b);
      stats_permutation_init(&permutation, vec_n, b);
      
      clock_gettime(CLOCK_REALTIME, &st);

      rrho_init_params_ea(&rrho, &params_ea);
      rrho_rectangle_min_ea(&rrho, 0, 0, vec_n, vec_n, &params_ea, RRHO_HYPER, 1, &rrho_coord); 
      rrho_hyper(&rrho, rrho_coord.i, rrho_coord.j, &rrho_res);
      // printf("after rrho_rectangle_min_ea()");
      
      rrho_permutation_generic(&rrho, 0, 0, vec_n, vec_n,
			       &params_ea, &permutation, RRHO_HYPER, 1, RRHO_EA, 96, rrho_res.pvalue, &perm_res);
      
      clock_gettime(CLOCK_REALTIME, &et);
      diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9d;
      
      stats_permutation_destroy(&permutation);
      rrho_destroy(&rrho);
      
      printf("%12.2F sec ((%zu, %zu), vec_perc= %zu, pvalue = %Le, count = %zu, padj = %Le)\n",
	     diff, rrho_coord.i, rrho_coord.j, vec_perc, rrho_res.pvalue, rrho_res.count, perm_res.pvalue);      
      fprintf(file, "%d\t%zu\t%e\n", num_threads, vec_n, diff); 
       
      free(a);
      free(b);
    }

  fclose(file);

  return EXIT_SUCCESS;
}
