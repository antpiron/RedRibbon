#ifndef __ERROR_H
#define __ERROR_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>


#define ERR_SUCCESS 0
#define ERR_UNDEF   1
#define ERR_ERRNO   2
#define ERR_FERROR  3
#define ERR_GAI     4
#define ERR_CUSTOM  5
#define ERR_LAST   16

struct error_st {
  int type;
  int errnum;
};

struct error_st* error_get_errno();

#define ale_error (*error_get_errno())

#define ERROR_RET(test,val)			\
  do						\
    {						\
      if (test)					\
	  return (val);				\
    }						\
  while (0)

#define ERROR_GOTO(test,label)			\
  do						\
    {						\
      if (test)					\
	  goto label;				\
    }						\
  while (0)

#define SUCCESS_RET(test)						\
  do									\
    {									\
      if (test)								\
	{								\
	  return ERR_SUCCESS;						\
	}								\
    }									\
  while (0)


#define ERROR_TYPE_RET(test,typ,err,val)	\
  do						\
    {						\
      if (test)					\
	{					\
	  ale_error.type = (typ);			\
	  ale_error.errnum = (err);			\
	  return (val);				\
	}					\
    }						\
  while (0)

#define ERROR_ERRNO_RET(test,val) ERROR_TYPE_RET((test),ERR_ERRNO,(errno),(val))
#define ERROR_GAI_RET(test,val) ERROR_TYPE_RET((test),ERR_GAI,0,(val))
#define ERROR_FERROR_RET(test,val) ERROR_TYPE_RET((test),ERR_FERROR,0,(val))
#define ERROR_UNDEF_RET(test,val) ERROR_TYPE_RET((test),ERR_UNDEF,0,(val))
#define ERROR_CUSTOM_RET(test,err,val) ERROR_TYPE_RET((test),ERR_CUSTOM,(err),(val))

#define ERROR_TYPE_GOTO(test,typ,err,label)	\
  do						\
    {						\
      if (test)					\
	{					\
	  ale_error.type = (typ);			\
	  ale_error.errnum = (err);			\
	  goto label;				\
	}					\
    }						\
  while (0)

#define ERROR_ERRNO_GOTO(test,label) ERROR_TYPE_GOTO((test),ERR_ERRNO,(errno),label)
#define ERROR_GAI_GOTO(test,label) ERROR_TYPE_GOTO((test),ERR_GAI,0,label)
#define ERROR_FERROR_GOTO(test,label) ERROR_TYPE_GOTO((test),ERR_FERROR,0,label)
#define ERROR_UNDEF_GOTO(test,label) ERROR_TYPE_GOTO((test),ERR_UNDEF,0,label)
#define ERROR_CUSTOM_GOTO(test,err,label) ERROR_TYPE_GOTO((test),ERR_CUSTOM,(err),label)


#define ERROR_PRINTF(fmt,...)						\
  do									\
    {									\
      fprintf(stderr, fmt, __VA_ARGS__);				\
      if (ERR_ERRNO == ale_error.type)					\
	{								\
	  const size_t buflen = 1024;					\
	  char buf[buflen+1];						\
	  buf[buflen] = 0;						\
	  strerror_r(ale_error.errnum, buf, buflen);			\
	  fprintf(stderr, "%s\n", buf);					\
	}								\
      else if (ERR_FERROR == ale_error.type)				\
	fprintf(stderr, "%s\n", "file error\n");			\
      else if (ERR_GAI == ale_error.type)					\
	fprintf(stderr, "%s\n", "gai error\n");				\
      else if (ERR_UNDEF == ale_error.type)					\
	fprintf(stderr, "%s\n", "undefined error\n");			\
      else if (ERR_CUSTOM == ale_error.type)				\
	fprintf(stderr, "Custom error %d\n", ale_error.errnum);		\
    }									\
  while (0)
  
#define ERROR_MSG_FMT(test,fmt,...)		\
  do						\
    {						\
      if (test)					\
	{					\
	  ERROR_PRINTF(fmt, __VA_ARGS__);	\
	}					\
    }						\
  while (0)


#define ERROR_MSG(test,str) ERROR_MSG_FMT((test),"%s",(str))

#define ERROR_TYPE_MSG_FMT(test,typ,err,fmt,...)	\
  do							\
    {							\
      if (test)						\
	{						\
	  ale_error.type = (typ);				\
	  ale_error.errnum = (err);				\
	  ERROR_PRINTF(fmt, __VA_ARGS__);		\
	}						\
    }							\
  while (0)

#define ERROR_TYPE_MSG(test,typ,err,str) ERROR_TYPE_MSG_FMT((test),(typ),(err),"%s",(str))

#define ERROR_ERRNO_MSG_FMT(test,fmt,...)  ERROR_TYPE_MSG_FMT((test),ERR_ERRNO,errno,fmt, __VA_ARGS__)
#define ERROR_GAI_MSG_FMT(test,fmt,...)    ERROR_TYPE_MSG_FMT((test),ERR_GAI,0,fmt, __VA_ARGS__)
#define ERROR_FERROR_MSG_FMT(test,fmt,...) ERROR_TYPE_MSG_FMT((test),ERR_FERROR,0,fmt, __VA_ARGS__)
#define ERROR_UNDEF_MSG_FMT(test,fmt,...)  ERROR_TYPE_MSG_FMT((test),ERR_UNDEF,0,fmt, __VA_ARGS__)
#define ERROR_CUSTOM_MSG_FMT(test,err,fmt,...)  ERROR_TYPE_MSG_FMT((test),ERR_CUSTOM,(err),fmt, __VA_ARGS__)

#define ERROR_ERRNO_MSG(test,str)  ERROR_TYPE_MSG((test),ERR_ERRNO,errno,str)
#define ERROR_GAI_MSG(test,str)    ERROR_TYPE_MSG((test),ERR_GAI,0,str)
#define ERROR_FERROR_MSG(test,str) ERROR_TYPE_MSG((test),ERR_FERROR,0,str)
#define ERROR_UNDEF_MSG(test,str)  ERROR_TYPE_MSG((test),ERR_UNDEF,0,str)
#define ERROR_CUSTOM_MSG(test,err,str)  ERROR_TYPE_MSG((test),(err),ERR_CUSTOM,str)


#define ERROR_FATAL_FMT(test,fmt,...)		\
  do						\
    {						\
      if (test)					\
	{					\
	  ERROR_PRINTF(fmt, __VA_ARGS__);	\
	  exit(EXIT_FAILURE);			\
	}					\
    }						\
  while (0)


#define ERROR_FATAL(test,str) ERROR_FATAL_FMT((test),"%s",(str))

#define ERROR_TYPE_FATAL_FMT(test,typ,err,fmt,...)	\
  do							\
    {							\
      if (test)						\
	{						\
	  ale_error.type = (typ);				\
	  ale_error.errnum = (err);				\
	  ERROR_FATAL_FMT(1,fmt, __VA_ARGS__);		\
	}						\
    }							\
  while (0)

#define ERROR_TYPE_FATAL(test,typ,err,str) ERROR_TYPE_FATAL_FMT((test),(typ),(err),"%s",(str))

#define ERROR_ERRNO_FATAL_FMT(test,fmt,...)  ERROR_TYPE_FATAL_FMT((test),ERR_ERRNO,errno,fmt, __VA_ARGS__)
#define ERROR_GAI_FATAL_FMT(test,fmt,...)    ERROR_TYPE_FATAL_FMT((test),ERR_GAI,0,fmt, __VA_ARGS__)
#define ERROR_FERROR_FATAL_FMT(test,fmt,...) ERROR_TYPE_FATAL_FMT((test),ERR_FERROR,0,fmt, __VA_ARGS__)
#define ERROR_UNDEF_FATAL_FMT(test,fmt,...)  ERROR_TYPE_FATAL_FMT((test),ERR_UNDEF,0,fmt, __VA_ARGS__)
#define ERROR_CUSTOM_FATAL_FMT(test,err,fmt,...)  ERROR_TYPE_FATAL_FMT((test),ERR_CUSTOM,(err),fmt, __VA_ARGS__)

#define ERROR_ERRNO_FATAL(test,str)  ERROR_TYPE_FATAL((test),ERR_ERRNO,errno,str)
#define ERROR_GAI_FATAL(test,str)    ERROR_TYPE_FATAL((test),ERR_GAI,0,str)
#define ERROR_FERROR_FATAL(test,str) ERROR_TYPE_FATAL((test),ERR_FERROR,0,str)
#define ERROR_UNDEF_FATAL(test,str)  ERROR_TYPE_FATAL((test),ERR_UNDEF,0,str)
#define ERROR_CUSTOM_FATAL(test,err,str)  ERROR_TYPE_FATAL((test),(err),ERR_CUSTOM,str)

#define ERROR_SYSLOG(level,fmt,...)					\
  do									\
    {									\
      int lev = (level);						\
      if (ERR_ERRNO == ale_error.type)					\
	{								\
	  char buf[4096] = "";						\
	  strerror_r(errno, buf, 4096);					\
	  syslog(lev, fmt": %s", __VA_ARGS__, buf);			\
	}								\
      else if (ERR_FERROR == ale_error.type)				\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "file error\n");		\
      else if (ERR_GAI == ale_error.type)					\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "gai error\n");		\
      else if (ERR_UNDEF == ale_error.type)					\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "undefined error\n");	\
      else if (ERR_CUSTOM == ale_error.type)				\
	syslog(lev, fmt"%s %d\n", __VA_ARGS__, "custom error\n", ale_error.errnum);	\
    }									\
  while (0)

#define ERROR_LOG_FMT(test,level,fmt,...)				\
  do									\
    {									\
      if (test)								\
	ERROR_SYSLOG((level), fmt, __VA_ARGS__);			\
    } while (0)


#define ERROR_LOG(test,level,str) ERROR_LOG_FMT((test),(level),"%s",(str))


#endif
