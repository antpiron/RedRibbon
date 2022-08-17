#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <omp.h>

#include <ale.h>
#include "cRedRibbon.h"

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

  file = fopen(".perf/p-rrho-permutation.tsv", "w");
  ERROR_ERRNO_FATAL(NULL == file, "fopen() failed.\n");
  fprintf(file, "n.threads\tn\tstep\ttime\n");
  
  for (size_t iter = 1 ; iter <= 32 ; iter *= 2)
    {
      size_t vec_n = iter * 5000;
      double *a = malloc(vec_n * sizeof(double));
      double *b = malloc(vec_n * sizeof(double));
      size_t m = ceil(sqrt(vec_n)), n = ceil(sqrt(vec_n));
      
      printf("RRHO for vectors of n = %6zu elements, step = %4ld, n.threads = %3d: ", vec_n, vec_n / n, num_threads);
	
      for (size_t i = 0 ; i < vec_n ; i++)
	{
	  a[i] = stats_unif_std_rand();
	  b[i] = stats_unif_std_rand();
	}
      
      rrho_init(&rrho, vec_n, a, b);
      stats_permutation_init(&permutation, n, b);
      
      clock_gettime(CLOCK_REALTIME, &st);
      
      rrho_rectangle_min(&rrho, 0, 0, vec_n, vec_n, &(struct rrho_rectangle_params){.m = m, .n = n}, RRHO_HYPER, 1, &rrho_coord);
      rrho_hyper(&rrho, rrho_coord.i, rrho_coord.j, &rrho_res);
      rrho_permutation_generic(&rrho, 0, 0, vec_n, vec_n,
			       &(struct rrho_rectangle_params){.m = m, .n = n}, &permutation, 
			       RRHO_HYPER, 1, RRHO_CLASSIC, 96, rrho_res.pvalue, &perm_res);
      
      clock_gettime(CLOCK_REALTIME, &et);
      diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9d;
      
      stats_permutation_destroy(&permutation);
      rrho_destroy(&rrho);
      
      printf("%12.2F sec (pvalue = %Le, padj = %Le)\n", diff, rrho_res.pvalue, perm_res.pvalue);      
      fprintf(file, "%d\t%zu\t%zu\t%e\n", num_threads, vec_n, vec_n / n, diff); 
       
      free(a);
      free(b);
    }

  /* from RRHO paper
     For a comparison of gene lists of length 5000 and a rank threshold step size of 50, it takes ∼15 s to calculate the 10 000 [(5000/50)2] hypergeometric CDF results required to create one RRHO map using an Intel Xeon 3.2 GHz processor, thus requiring ∼4 h to create and analyze 1000 permutation maps on a single computational node.
  */
  size_t vec_n =  5000;
  double *a = malloc(vec_n * sizeof(double));
  double *b = malloc(vec_n * sizeof(double));
  size_t m = 100, n = 100;
  
  for (size_t i = 0 ; i < vec_n ; i++)
    {
      a[i] = stats_unif_std_rand();
      b[i] = stats_unif_std_rand();
    }

  rrho_init(&rrho, vec_n, a, b);    
  stats_permutation_init(&permutation, n, b);
   
  clock_gettime(CLOCK_REALTIME, &st);
  
  rrho_rectangle_min(&rrho, 0, 0, vec_n, vec_n, &(struct rrho_rectangle_params){.m = m, .n = n}, RRHO_HYPER, 1, &rrho_coord);
  rrho_hyper(&rrho, rrho_coord.i, rrho_coord.j, &rrho_res);
  rrho_permutation_generic(&rrho, 0, 0, vec_n, vec_n, &(struct rrho_rectangle_params){.m = m, .n = n},
			   &permutation, RRHO_HYPER, 1, RRHO_CLASSIC, 96, rrho_res.pvalue, &perm_res);
  
  clock_gettime(CLOCK_REALTIME, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9d;
  
  stats_permutation_destroy(&permutation);
  rrho_destroy(&rrho);

  printf("RRHO for vectors of n = %6zu elements, step = %4ld, n.threads = %3d: ", vec_n, vec_n / n, omp_get_max_threads());
  printf("%12.2F sec (pvalue = %Le, padj = %Le)\n", diff, rrho_res.pvalue, perm_res.pvalue);
  fprintf(file, "%d\t%zu\t%zu\t%e\n", omp_get_max_threads(), vec_n, vec_n / n, diff); 

  /* Mono-thread 10k */
  omp_set_num_threads(1);
  rrho_init(&rrho, vec_n, a, b);    
  stats_permutation_init(&permutation, n, b);
   
  clock_gettime(CLOCK_REALTIME, &st);
  
  rrho_rectangle_min(&rrho, 0, 0, vec_n, vec_n, &(struct rrho_rectangle_params){.m = m, .n = n}, RRHO_HYPER, 1, &rrho_coord);
  rrho_hyper(&rrho, rrho_coord.i, rrho_coord.j, &rrho_res);
  rrho_permutation_generic(&rrho, 0, 0, vec_n, vec_n,
			   &(struct rrho_rectangle_params){.m = m, .n = n},
			   &permutation, RRHO_HYPER, 1, RRHO_CLASSIC, 96, rrho_res.pvalue, &perm_res);
  
  clock_gettime(CLOCK_REALTIME, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9d;
  
  stats_permutation_destroy(&permutation);
  rrho_destroy(&rrho);

  printf("RRHO for vectors of n = %6zu elements, step = %4ld, n.threads = %3d: ", vec_n, vec_n / n, omp_get_max_threads());
  printf("%12.2F sec (pvalue = %Le, padj = %Le)\n", diff, rrho_res.pvalue, perm_res.pvalue);
  fprintf(file, "%d\t%zu\t%zu\t%e\n", omp_get_max_threads(), vec_n, vec_n / n, diff); 

  fclose(file);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
