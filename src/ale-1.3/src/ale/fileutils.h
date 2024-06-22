#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <ale/vector.h>


int rmpath(const char *dname, const char *pathname);
int unlink_path(const char *dname, const char *pathname);
int mkpath(const char *pathname, mode_t mode);
FILE* mkpath_fopen(const char *pathname, const char *mode);
int is_gzip(const char *filename);
FILE* gzfopen(const char *pathname, const char *mode);

VECTOR_INIT(int,char*)

struct csv
{
  void *scanner;
};

int csv_init(struct csv *csv, FILE *file);
void csv_destroy(struct csv *csv);
int csv_readline(struct csv *csv, struct vector_int *vector);


#endif
