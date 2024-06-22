#include <pthread.h>

#include "ale/error.h"

static pthread_key_t errno_key;
static pthread_once_t errno_once = PTHREAD_ONCE_INIT;


static void
error_free(void *ptr)
{
  free(ptr);
  pthread_setspecific(errno_key, NULL);
}

static void 
error_init(void)
{ 
  pthread_key_create(&errno_key, error_free); 
}


struct error_st* 
error_get_errno()
{
  struct error_st *err = NULL;

  pthread_once(&errno_once, error_init);

  err = pthread_getspecific(errno_key);
  if (!err)
  {
    err = malloc(sizeof(struct error_st));
    pthread_setspecific(errno_key, err);
  }

  return err;
}

