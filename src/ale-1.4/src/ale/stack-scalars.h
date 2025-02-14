#ifndef __STACK_SCALARS_H
#define __STACK_SCALARS_H

#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
#include <ale/stack.h>

STACK_INIT(char,char)
STACK_INIT(short,short)
STACK_INIT(int,int)
STACK_INIT(long,long)
STACK_INIT(unsigned_char,unsigned char)
STACK_INIT(unsigned_short,unsigned short)
STACK_INIT(unsigned_int,unsigned int)
STACK_INIT(unsigned_long,unsigned long)

STACK_INIT(size_t,size_t)
STACK_INIT(ssize_t,ssize_t)
     
STACK_INIT(intptr_t,intptr_t)
STACK_INIT(uintptr_t,uintptr_t)

STACK_INIT(int8_t,int8_t)
STACK_INIT(uint8_t,uint8_t)
STACK_INIT(int16_t,int16_t)
STACK_INIT(uint16_t,uint16_t)
STACK_INIT(int32_t,int32_t)
STACK_INIT(uint32_t,uint32_t)
STACK_INIT(int64_t,int64_t)
STACK_INIT(uint64_t,uint64_t)

STACK_INIT(float,float)
STACK_INIT(double,double)
STACK_INIT(long_double,long double)

#endif
